#include "map.hpp"

using Location = std::pair<int, int>;
using FrontierElement = std::pair<double, Location>;

namespace std {
    template <> struct hash<Location> {
        std::size_t operator()(const Location& id) const noexcept {
            return std::hash<int>()(id.first ^ (id.second << 16));
        }
    };
}

namespace Game {
    namespace States {
        const std::string Map::logChannel = "states.map";

        Map::Map() {
            this->logger = Services::Locator::LoggerService()->GetLogger(Map::logChannel);
            this->state = State::Gameplay;
            this->previousState = State::Gameplay;
            this->pop = false;
            this->scene = std::make_shared<Scenes::Scene>(this);
            this->camera = std::make_shared<Scenes::Camera>(0.0f, 0.0f, static_cast<float>(Services::Locator::VideoService()->GetScreenWidth()), static_cast<float>(Services::Locator::VideoService()->GetScreenHeight()));
            this->LoadLuaState("scripts/states/map.lua");

            // TODO: Figure out a better place to put this. Really we should probably be calling LoadMap in
            // WorldManager::NewGame.
            this->currentMap = Services::Locator::WorldService()->GetWorld()->currentMap;
            this->currentMapEncounterArea = nullptr;
            this->luaState->script_file("scripts/maps/" + this->currentMap->name + ".lua");
            sol::protected_function spawnNPCs = (*this->luaState.get())["spawn_npcs"];

            if (spawnNPCs.valid()) {
                sol::protected_function_result result = spawnNPCs();

                if (!result.valid()) {
                    sol::error err = result;
                    this->logger->error() << "Error in Lua spawn_npcs function: " << err.what();
                }
            }

            // TODO: This should all also probably be a function which is called by WorldManager::NewGame.
            std::shared_ptr<Scenes::Actor> player = this->scene->AddActor<Scenes::Controllers::PlayerController>("player", "Sean", Services::Locator::WorldService()->GetWorld()->playerParty->GetLeader()->GetSpritesheet()->name, "", Services::Locator::WorldService()->GetWorld()->playerCurrentX, Services::Locator::WorldService()->GetWorld()->playerCurrentY, Scenes::Actor::Direction::Down, "", "");
            player->SetPersistent(true);
            player->SetMovementSpeed(4.0f);
            this->scene->actorManager->player = player;
            this->camera->Follow(this->scene->actorManager->player);

            // TODO: Create some sort of wrapper around this state.
            this->movementInputHeldDirection = Scenes::Actor::Direction::Down;
            this->movementInputHeld = false;
            this->dialogueChoiceInputTimer = 0.0f;
        }

        Map::~Map() {
        }

        void Map::HandleEvent(const SDL_Event& event) {
        }

        Transition Map::Update(const float deltaTime) {
            switch (this->state) {
            case State::Gameplay:
                return this->UpdateGameplay(deltaTime);
            case State::Dialogue:
                return this->UpdateDialogue(deltaTime);
            case State::Cutscene:
                return this->UpdateCutscene(deltaTime);
            default:
                return Transition::Pop();
            }
        }

        Transition Map::UpdateGameplay(const float deltaTime) {
            this->UpdateMovementInput();

            this->scene->Update(deltaTime);

            this->scene->ProcessCompletedSteps();

            if (!this->scene->actorManager->player->IsMoving() && this->movementInputHeld) {
                this->scene->actorManager->player->ClearPendingMovement();

                this->QueueMovement(this->scene->actorManager->player.get(), this->movementInputHeldDirection, 1);
            }

            this->scene->ProcessPendingMovement();

            if (Services::Locator::InputService()->GetCurrentInputState().confirmPressed) {
                if (this->TryInteract()) {
                    this->logger->debug() << "Player interacted with something.";
                } else {
                    this->logger->debug() << "Player tried to interact but there was nothing to interact with.";
                }
            }

            this->camera->Update(deltaTime, this->currentMap->width * this->currentMap->tilewidth, this->currentMap->height * this->currentMap->tileheight);

            if (this->pop) {
                return Transition::Pop();
            }

            // TODO: Add a flag to check to see if the pause button has been pressed.
            // TODO: Add a check to see if the player has stepped on an encounter area and roll a dice
            // to see if a battle should be triggered. For now we're always going to be in the Map state.
            GameStateType nextState = GameStateType::map;

            switch (nextState) {
            case GameStateType::pause_menu:
                return Transition::Push(std::make_shared<PauseMenu>());
            case GameStateType::battle:
                return Transition::Push(std::make_shared<Battle>(this->currentMapEncounterArea));
            default:
                return Transition::None();
            }
        }

        Transition Map::UpdateDialogue(const float deltaTime) {
            if (Services::Locator::InputService()->GetCurrentInputState().upHeld) {
                if (this->dialogueChoiceInputTimer <= 0.0f) {
                    this->dialogueSession.PreviousChoice();

                    this->dialogueChoiceInputTimer = 0.25f;
                } else {
                    this->dialogueChoiceInputTimer -= deltaTime;
                }
            } else if (Services::Locator::InputService()->GetCurrentInputState().downHeld) {
                if (this->dialogueChoiceInputTimer <= 0.0f) {
                    this->dialogueSession.NextChoice();

                    this->dialogueChoiceInputTimer = 0.25f;
                } else {
                    this->dialogueChoiceInputTimer -= deltaTime;
                }
            } else {
                this->dialogueChoiceInputTimer = 0.0f;
            }

            if (Services::Locator::InputService()->GetCurrentInputState().confirmPressed) {
                this->dialogueSession.Next();
            }

            this->dialogueSession.Update(deltaTime);

            if (this->dialogueSession.completed) {
                this->state = this->previousState;
            }

            return Transition::None();
        }

        Transition Map::UpdateCutscene(const float deltaTime) {
            this->cutsceneSession.Update(deltaTime);

            this->scene->Update(deltaTime);

            this->scene->ProcessPendingMovement();

            if (this->cutsceneSession.IsCompleted()) {
                this->state = State::Gameplay;
            }

            return Transition::None();
        }

        void Map::UpdateMovementInput() {
            const bool* keyboardState = SDL_GetKeyboardState(nullptr);

            if (keyboardState[SDL_SCANCODE_UP]) {
                this->movementInputHeld = true;
                this->movementInputHeldDirection = Scenes::Actor::Direction::Up;
            } else if (keyboardState[SDL_SCANCODE_RIGHT]) {
                this->movementInputHeld = true;
                this->movementInputHeldDirection = Scenes::Actor::Direction::Right;
            } else if (keyboardState[SDL_SCANCODE_DOWN]) {
                this->movementInputHeld = true;
                this->movementInputHeldDirection = Scenes::Actor::Direction::Down;
            } else if (keyboardState[SDL_SCANCODE_LEFT]) {
                this->movementInputHeld = true;
                this->movementInputHeldDirection = Scenes::Actor::Direction::Left;
            } else {
                this->movementInputHeld = false;
            }
        }

        std::string Map::ProcessInput(const Input::Button key) {
            return "";
        }

        void Map::Render() {
            this->currentMap->Render(this->camera->x, this->camera->y);

            this->scene->Render();

            if (this->state == State::Dialogue) {
                this->dialogueSession.Render(this->camera);
            }
        }

        void Map::QueueMovement(Scenes::Actor* actor, const Scenes::Actor::Direction direction, const int distance) {
            // Eventually this do pathfinding to enqueue the steps. For now this just enqueues distance number
            // of steps in the direction specificed. Eventually there will probably be an overloaded version
            // of this function that takes a target tile and does pathfinding to get there and enqueues the
            // necessary steps to get there.
            for (int step = 0; step < distance; step++) {
                this->logger->debug() << "Enqueuing step for " << actor->name << " in direction " << static_cast<int>(direction) << ".";

                actor->QueueMovement(direction);
            }
        }

        std::vector<Scenes::Actor::Direction> Map::Pathfind(Scenes::Actor* actor, const int targetX, const int targetY) {
            std::priority_queue<FrontierElement, std::vector<FrontierElement>, std::greater<FrontierElement>> frontier;
            std::unordered_map<Location, Location> cameFrom;
            std::unordered_map<Location, double> costSoFar;
            Location start = std::make_pair(actor->GetCurrentTileX(), actor->GetCurrentTileY());

            frontier.emplace(0.0, start);
            cameFrom[start] = start;
            costSoFar[start] = 0.0;

            while (!frontier.empty()) {
                Location current = frontier.top().second;

                frontier.pop();

                if (current.first == targetX && current.second == targetY) {
                    break;
                }

                std::vector<Location> potentialNeighbors = {
                    {current.first, current.second - 1},
                    {current.first + 1, current.second},
                    {current.first, current.second + 1},
                    {current.first - 1, current.second}};

                std::vector<Location> neighbors;

                for (Location potentialNeighbor : potentialNeighbors) {
                    if (potentialNeighbor.first < 0 || potentialNeighbor.first >= this->currentMap->width || potentialNeighbor.second < 0 || potentialNeighbor.second >= this->currentMap->height) {
                        continue;
                    }

                    if (!this->currentMap->GetWalkability(potentialNeighbor.first, potentialNeighbor.second)) {
                        continue;
                    }

                    if (this->scene->IsTileBlocked(potentialNeighbor.first, potentialNeighbor.second, actor)) {
                        continue;
                    }

                    neighbors.push_back(potentialNeighbor);
                }

                for (const auto& next : neighbors) {
                    double newCost = costSoFar[current] + 1.0;

                    if (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next]) {
                        costSoFar[next] = newCost;
                        double priority = newCost + std::hypot(targetX - next.first, targetY - next.second);
                        frontier.emplace(priority, next);
                        cameFrom[next] = current;
                    }
                }
            }

            std::vector<Scenes::Actor::Direction> path;
            Location current = std::make_pair(targetX, targetY);

            if (cameFrom.find(current) == cameFrom.end()) {
                return {};
            }

            while (current != start) {
                Location previous = cameFrom[current];

                if (previous.first == current.first) {
                    if (previous.second < current.second) {
                        path.push_back(Scenes::Actor::Direction::Down);
                    } else {
                        path.push_back(Scenes::Actor::Direction::Up);
                    }
                } else if (previous.second == current.second) {
                    if (previous.first < current.first) {
                        path.push_back(Scenes::Actor::Direction::Right);
                    } else {
                        path.push_back(Scenes::Actor::Direction::Left);
                    }
                }

                current = previous;
            }

            std::reverse(path.begin(), path.end());

            return path;
        }

        bool Map::CanMove(Scenes::Actor* actor, const Scenes::Actor::Direction direction) {
            int targetX = actor->GetCurrentTileX();
            int targetY = actor->GetCurrentTileY();

            switch (direction) {
            case Scenes::Actor::Direction::Up:
                targetY--;

                break;
            case Scenes::Actor::Direction::Right:
                targetX++;

                break;
            case Scenes::Actor::Direction::Down:
                targetY++;

                break;
            case Scenes::Actor::Direction::Left:
                targetX--;

                break;
            }

            if (targetX < 0 || targetX >= this->currentMap->width || targetY < 0 || targetY >= this->currentMap->height) {
                return false;
            }

            if (!this->currentMap->GetWalkability(targetX, targetY)) {
                return false;
            }

            if (this->scene->IsTileBlocked(targetX, targetY, actor)) {
                return false;
            }

            return true;
        }

        bool Map::TryInteract() {
            int targetX = this->scene->actorManager->player->GetOccupiedTileX();
            int targetY = this->scene->actorManager->player->GetOccupiedTileY();

            switch (this->scene->actorManager->player->GetDirection()) {
            case Scenes::Actor::Direction::Up:
                targetY--;

                break;
            case Scenes::Actor::Direction::Right:
                targetX++;

                break;
            case Scenes::Actor::Direction::Down:
                targetY++;

                break;
            case Scenes::Actor::Direction::Left:
                targetX--;

                break;
            }

            if (targetX < 0 || targetX >= this->currentMap->width || targetY < 0 || targetY >= this->currentMap->height) {
                return false;
            }

            auto actor = this->GetActorAtTile(targetX, targetY);

            if (actor.has_value()) {
                if (actor.value().get() == this->scene->actorManager->player.get()) {
                    return false;
                }

                actor.value()->Interact(this->scene->actorManager->player);

                return true;
            }

            //std::vector<std::shared_ptr<Objects::Maps::MapObject>> objects = this->currentMap->GetObjects(targetX, targetY);

            //for (auto object = objects.begin(); object != objects.end(); object++) {
            //    if ((*object)->GetType() == "interactable") {
            //        (*this->luaState.get())["on_interact"](*object);
            //        return true;
            //    }
            //}

            return false;
        }

        std::optional<std::shared_ptr<Scenes::Actor>> Map::GetActorAtTile(const int x, const int y) const {
            for (auto& actor : this->scene->actorManager->actors) {
                if (actor->OccupiesTile(x, y)) {
                    return actor;
                }
            }

            return std::nullopt;
        }

        bool Map::LoadMap(const std::string& mapName, const int startX, const int startY) {
            if (this->state == State::Gameplay) {
                this->scene->actorManager->actors.erase(std::remove_if(this->scene->actorManager->actors.begin(), this->scene->actorManager->actors.end(), [](const std::shared_ptr<Scenes::Actor>& actor) {
                                       return !actor->IsPersistent();
                                   }),
                                   this->scene->actorManager->actors.end());

                std::erase_if(this->scene->actorManager->actorLookup, [](const auto& pair) {
                    return !pair.second->IsPersistent();
                });
            }

            Services::Locator::WorldService()->GetWorld()->LoadMap(mapName);

            this->currentMap = Services::Locator::WorldService()->GetWorld()->currentMap;
            
            Services::Locator::WorldService()->UpdatePlayerPosition(startX, startY);

            this->scene->PlaceActor(this->scene->actorManager->player, startX, startY, Scenes::Actor::Direction::Down);

            this->camera->Follow(this->scene->actorManager->player);

            (*this->luaState.get())["after_map_load"]();

            this->luaState->script_file("scripts/maps/" + this->currentMap->name + ".lua");
            sol::protected_function spawnNPCs = (*this->luaState.get())["spawn_npcs"];

            if (spawnNPCs.valid()) {
                sol::protected_function_result result = spawnNPCs();

                if (!result.valid()) {
                    sol::error e = result;
                    this->logger->error() << "Error in Lua spawn_npcs function: " << e.what();
                }
            }

            return true;
        }

        bool Map::UnloadMap() {
            Services::Locator::WorldService()->GetWorld()->UnloadMap();

            this->currentMap = Services::Locator::WorldService()->GetWorld()->currentMap;

            if (this->currentMap) {
                return false;
            }

            //(*this->luaState.get())["after_map_load"]();

            return false;
        }

        std::shared_ptr<Objects::Maps::Map> Map::GetCurrentMap() {
            return this->currentMap;
        }

        std::shared_ptr<Objects::Maps::MapEncounterArea> Map::GetCurrentMapEncounterArea(const int x, const int y) {
            std::vector <std::shared_ptr<Objects::Maps::MapObject>> objects = this->currentMap->GetObjects(x, y);

            for (auto object = objects.begin(); object != objects.end(); object++) {
                if ((*object)->GetType() == "encounter_area") {
                    return std::static_pointer_cast<Objects::Maps::MapEncounterArea>(*object);
                }
            }

            return nullptr;
        }

        void Map::SetCurrentMapEncounterArea(Objects::Maps::MapObject* mapEncounterArea) {
            this->currentMapEncounterArea = dynamic_cast<Objects::Maps::MapEncounterArea*>(mapEncounterArea);
        }

        void Map::Step(unsigned int x, unsigned int y) {
            auto objects = this->currentMap->GetObjects(x, y);

            for (auto object = objects.begin(); object != objects.end(); object++) {
                Objects::Maps::MapLoadPoint* mapLoadPoint = dynamic_cast<Objects::Maps::MapLoadPoint*>(object->get());

                if (mapLoadPoint) {
                    // TODO: Make the MapLoadPoint parser handle this.
                    const int startX = mapLoadPoint->GetProperty("x") != "" ? std::stoi(mapLoadPoint->GetProperty("x")) : 0;
                    const int startY = mapLoadPoint->GetProperty("y") != "" ? std::stoi(mapLoadPoint->GetProperty("y")) : 0;

                    this->LoadMap(mapLoadPoint->GetProperty("map"), startX, startY);

                    break;
                }

                Objects::Maps::CutsceneTrigger* cutsceneTrigger = dynamic_cast<Objects::Maps::CutsceneTrigger*>(object->get());

                if (cutsceneTrigger) {
                    this->logger->debug() << "Player stepped on a cutscene trigger with cutscene ID \"" << cutsceneTrigger->GetCutsceneId() << "\".";

                    this->StartCutscene(cutsceneTrigger->GetCutsceneId());

                    break;
                }
            }
        }

        void Map::StartDialogue(const std::string& dialogueId) {
            this->logger->debug() << "Starting dialogue with ID \"" << dialogueId << "\".";

            std::shared_ptr<Scenes::Dialogue::DialogueGraph> graph = std::make_shared<Scenes::Dialogue::DialogueGraph>(dialogueId);

            this->dialogueSession.Start(graph);

            this->previousState = this->state;
            this->state = State::Dialogue;
        }

        bool Map::DialogueSessionCompleted() const {
            return this->dialogueSession.completed;
        }

        void Map::StartCutscene(const std::string& cutsceneId) {
            this->logger->debug() << "Starting cutscene with ID \"" << cutsceneId << "\".";

            this->scene->actorManager->player->ClearPendingMovement();

            std::shared_ptr<Scenes::Cutscenes::Cutscene> cutscene = std::make_shared<Scenes::Cutscenes::Cutscene>(this, cutsceneId);

            this->cutsceneSession.Start(cutscene);

            this->previousState = this->state;
            this->state = State::Cutscene;
        }

        std::shared_ptr<Scenes::Actor> Map::AddActorAtSpawnPoint(const std::string& id, const std::string& name, const std::string& spritesheetName, const std::string& dialogueId, const std::string& spawnPointName, const Scenes::Actor::Direction direction, const std::string& movementScriptName, const std::string& interactionScriptName) {
            std::shared_ptr<Objects::Maps::SpawnPoint> spawnPoint = this->currentMap->GetSpawnPoint(spawnPointName);

            if (!spawnPoint) {
                this->logger->error() << "Failed to add actor \"" << id << "\". Spawn point \"" << spawnPointName << "\" not found.";

                return nullptr;
            }

            return this->scene->AddActor<Scenes::Controllers::ScriptedController>(id, name, spritesheetName, dialogueId, spawnPoint->x, spawnPoint->y, direction, movementScriptName, interactionScriptName);
        }

        void Map::LoadLuaState(const std::string& scriptFilePath) {
            Base::LoadLuaState(scriptFilePath);

            this->luaState->open_libraries(sol::lib::math, sol::lib::os);

            Objects::Text::LuaInterface::Bind(this->luaState);
            Objects::Image::LuaInterface::Bind(this->luaState);
            Objects::Maps::Map::LuaInterface::Bind(this->luaState);
            Objects::Maps::MapObject::LuaInterface::Bind(this->luaState);
            Objects::Maps::MapEncounterArea::LuaInterface::Bind(this->luaState);
            Map::LuaInterface::Bind(this->luaState);

            this->luaState->set("map_state", this);

            this->logger->debug() << "Loading \"" << scriptFilePath << "\".";

            this->luaState->script_file(scriptFilePath);

            this->logger->debug() << "Loaded \"" << scriptFilePath << "\".";

            luaState->new_enum("Direction",
                               "Up", Scenes::Actor::Direction::Up,
                               "Right", Scenes::Actor::Direction::Right,
                               "Down", Scenes::Actor::Direction::Down,
                               "Left", Scenes::Actor::Direction::Left);

            sol::protected_function initialize = (*this->luaState.get())["initialize"];

            if (initialize.valid()) {
                sol::protected_function_result result = initialize();

                if (!result.valid()) {
                    sol::error err = result;
                    this->logger->error() << "Error in Lua initialize function: " << err.what();
                }
            }
        }

        void Map::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
            sol::table states = (*luaState.get())["states"].get_or_create<sol::table>(sol::new_table());

            states.new_usertype<Map>("Map",
                                     sol::no_constructor,
                                     "startDialogue", &Map::StartDialogue,
                                     "startCutscene", &Map::StartCutscene,
                                     "addActorAtSpawnPoint", &Map::AddActorAtSpawnPoint
                                     );
        }
    }
}
