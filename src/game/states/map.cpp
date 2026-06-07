#include "map.hpp"

namespace Game {
    namespace States {
        const std::string Map::logChannel = "states.map";

        Map::Map() {
            this->logger = Services::Locator::LoggerService()->GetLogger(Map::logChannel);
            this->state = State::Gameplay;
            this->previousState = State::Gameplay;
            this->pop = false;
            this->scene = std::make_shared<Scenes::Scene>(this);
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
            this->scene->camera->Follow(this->scene->actorManager->player);

            this->inputDebounceTimer = 0.0f;
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
            this->scene->Update(deltaTime);

            if (Services::Locator::InputService()->GetCurrentInputState().confirmPressed) {
                if (this->TryInteract()) {
                    this->logger->debug() << "Player interacted with something.";
                } else {
                    this->logger->debug() << "Player tried to interact but there was nothing to interact with.";
                }
            }

            this->ProcessPendingCommands();

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
                if (this->inputDebounceTimer <= 0.0f) {
                    this->dialogueSession.PreviousChoice();

                    this->inputDebounceTimer = 0.25f;
                } else {
                    this->inputDebounceTimer -= deltaTime;
                }
            } else if (Services::Locator::InputService()->GetCurrentInputState().downHeld) {
                if (this->inputDebounceTimer <= 0.0f) {
                    this->dialogueSession.NextChoice();

                    this->inputDebounceTimer = 0.25f;
                } else {
                    this->inputDebounceTimer -= deltaTime;
                }
            } else {
                this->inputDebounceTimer = 0.0f;
            }

            if (Services::Locator::InputService()->GetCurrentInputState().confirmPressed) {
                this->dialogueSession.Next();
            }

            this->dialogueSession.Update(deltaTime);

            if (this->dialogueSession.completed) {
                this->state = this->previousState;
            }

            this->ProcessPendingCommands();

            return Transition::None();
        }

        Transition Map::UpdateCutscene(const float deltaTime) {
            this->cutsceneSession.Update(deltaTime);

            this->scene->Update(deltaTime);

            if (this->cutsceneSession.IsCompleted()) {
                this->scene->SetActorController("player", std::make_unique<Scenes::Controllers::PlayerController>(this->scene->actorManager->player.get()));

                this->state = State::Gameplay;
            }
            
            this->ProcessPendingCommands();

            return Transition::None();
        }

        std::string Map::ProcessInput(const Input::Button key) {
            return "";
        }

        void Map::ProcessPendingCommands() {
            while (!this->pendingCommands.empty()) {
                auto command = std::move(this->pendingCommands.front());

                this->pendingCommands.pop();

                std::visit([this](auto&& cmd) {
                    using T = std::decay_t<decltype(cmd)>;

                    if constexpr (std::is_same_v<T, LoadMapCommand>) {
                        this->LoadMap(cmd.mapName, cmd.startX, cmd.startY);
                    } else if constexpr (std::is_same_v<T, StartDialogueCommand>) {
                        this->StartDialogue(cmd.dialogueId);
                    } else if constexpr (std::is_same_v<T, StartCutsceneCommand>) {
                        this->StartCutscene(cmd.cutsceneId);
                    }
                },
                command);
            }
        }

        void Map::Render() {
            this->currentMap->Render(this->scene->camera->x, this->scene->camera->y);

            this->scene->Render();

            if (this->state == State::Dialogue) {
                this->dialogueSession.Render(this->scene->camera);
            }
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

                this->scene->actorManager->player->Interact(actor.value());

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
                //this->scene->actorManager->actors.erase(std::remove_if(this->scene->actorManager->actors.begin(), this->scene->actorManager->actors.end(), [](const std::shared_ptr<Scenes::Actor>& actor) {
                //                       return !actor->IsPersistent();
                //                   }),
                //                   this->scene->actorManager->actors.end());

                //std::erase_if(this->scene->actorManager->actorLookup, [](const auto& pair) {
                //    return !pair.second->IsPersistent();
                //});
                std::vector<std::string> toRemove;

                for (auto& actor : scene->actorManager->actors) {
                    if (!actor->IsPersistent()) {
                        toRemove.push_back(actor->id);
                    }
                }

                for (auto& id : toRemove) {
                    scene->RemoveActor(id);
                }
            }

            Services::Locator::WorldService()->GetWorld()->LoadMap(mapName);

            this->currentMap = Services::Locator::WorldService()->GetWorld()->currentMap;
            
            Services::Locator::WorldService()->UpdatePlayerPosition(startX, startY);

            this->scene->PlaceActor(this->scene->actorManager->player, startX, startY, Scenes::Actor::Direction::Down);

            this->scene->camera->Follow(this->scene->actorManager->player);

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

        std::shared_ptr<Objects::Maps::Map> Map::GetCurrentMap() const {
            return this->currentMap;
        }

        std::shared_ptr<Objects::Maps::MapEncounterArea> Map::GetCurrentMapEncounterArea(const int x, const int y) const {
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

                    this->pendingCommands.push(LoadMapCommand{mapLoadPoint->GetProperty("map"), startX, startY});

                    break;
                }

                Objects::Maps::CutsceneTrigger* cutsceneTrigger = dynamic_cast<Objects::Maps::CutsceneTrigger*>(object->get());

                if (cutsceneTrigger) {
                    this->logger->debug() << "Player stepped on a cutscene trigger with cutscene ID \"" << cutsceneTrigger->GetCutsceneId() << "\".";

                    this->pendingCommands.push(StartCutsceneCommand{cutsceneTrigger->GetCutsceneId()});

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

            this->scene->SetActorController("player", std::make_unique<Scenes::Controllers::CutsceneController>(this->scene->actorManager->player.get()));

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

            states.new_usertype<StartDialogueCommand>("StartDialogueCommand",
                                                      sol::constructors<StartDialogueCommand(const std::string&)>(),
                                                      "dialogue_id", &StartDialogueCommand::dialogueId
                                                     );

            states.new_usertype<StartCutsceneCommand>("StartCutsceneCommand",
                                                      sol::constructors<StartCutsceneCommand(const std::string&)>(),
                                                      "cutscene_id", &StartCutsceneCommand::cutsceneId
                                                     );

            states.new_usertype<Map>("Map",
                                     sol::no_constructor,
                                     "queue_command", &Map::QueueCommand,
                                     "add_actor_at_spawn_point", &Map::AddActorAtSpawnPoint
                                     );
        }
    }
}
