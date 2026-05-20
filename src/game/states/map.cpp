#include "map.hpp"

namespace Game {
    namespace States {
        const std::string Map::logChannel = "states.map";

        Map::Map() {
            this->logger = Services::Locator::LoggerService()->GetLogger(Map::logChannel);
            this->pop = false;
            this->currentMap = Services::Locator::WorldService()->GetWorld()->currentMap;
            this->currentMapEncounterArea = nullptr;
            this->LoadLuaState("scripts/states/map.lua");
            this->camera = std::make_shared<Scene::Camera>(0.0f, 0.0f, static_cast<float>(Services::Locator::VideoService()->GetScreenWidth()), static_cast<float>(Services::Locator::VideoService()->GetScreenHeight()));
            this->player = std::make_shared<Scene::Actor>(Services::Locator::WorldService()->GetWorld()->playerParty->GetLeader()->GetSpritesheet());
            this->player->name = "Sean";
            this->actors.push_back(this->player);
            this->PlaceActor(this->player, Services::Locator::WorldService()->GetWorld()->playerCurrentX, Services::Locator::WorldService()->GetWorld()->playerCurrentY, Scene::Actor::Direction::Down);
            this->camera->Follow(this->player);

            // TODO: Load NPCs from NPC spawn points. For now just hardcoding them in because it's easier for testing.
            std::shared_ptr<Scene::Actor> casie = std::make_shared<Scene::Actor>(std::make_shared<Graphics::Spritesheet>("characters/casie"));
            casie->name = "Casie";
            std::shared_ptr<Scene::Actor> kyle = std::make_shared<Scene::Actor>(std::make_shared<Graphics::Spritesheet>("characters/kyle"));
            kyle->name = "Kyle";
            this->actors.push_back(casie);
            this->actors.push_back(kyle);
            this->PlaceActor(casie, 8, 10, Scene::Actor::Direction::Down);
            this->PlaceActor(kyle, 20, 4, Scene::Actor::Direction::Left);

            this->movementInputHeldDirection = Scene::Actor::Direction::Down;
            this->movementInputHeld = false;
        }

        Map::~Map() {
        }

        void Map::HandleEvent(const SDL_Event& event) {
            InputKey key = Services::Locator::InputService()->GetInputMapKey(event);

            if (key != InputKey::NO_KEY) {
                if (event.type == SDL_EVENT_KEY_DOWN) {
                    this->ProcessButtonDown(key);
                } else if (event.type == SDL_EVENT_KEY_UP) {
                    this->ProcessButtonUp(key);
                }
            }
        }

        Transition Map::Update(const double deltaTime) {
            std::string nextState = (*this->luaState.get())["update"](deltaTime);

            this->UpdateMovementInput();

            for (auto actor = this->actors.begin(); actor != this->actors.end(); actor++) {
                (*actor)->Update(deltaTime);
            }

            // TODO: Run this loop for each Actor.
            while (auto step = this->player->ConsumeCompletedStep()) {
                // Maybe only do this when transitioning to the PauseMenu state. It probably doesn't need to happen
                // every step and there's no reason to update it unless there's a chance we might save the game.
                Services::Locator::WorldService()->UpdatePlayerPosition(step->tileX, step->tileY);

                this->logger->debug() << "Player completed a step. New position: (" << step->tileX << ", " << step->tileY << ")";

                this->Step(step->tileX, step->tileY);
            }

            if (!this->player->IsMoving() && this->movementInputHeld) {
                this->player->ClearPendingMoves();

                this->Move(this->player.get(), this->movementInputHeldDirection, 1);
            }

            // TODO: Move NPCs here.

            for (auto& actor : this->actors) {
                if (!actor->IsMoving()) {
                    auto nextMove = actor->PeekMove();

                    if (nextMove.has_value()) {
                        actor->SetDirection(nextMove.value());

                        if (this->TryMove(actor.get(), nextMove.value())) {
                            actor->PopMove();
                            actor->BeginStep(nextMove.value());
                        }
                    }
                }
            }

            this->camera->Update(deltaTime, this->currentMap->width * this->currentMap->tilewidth, this->currentMap->height * this->currentMap->tileheight);

            if (this->pop) {
                return Transition::Pop();
            }

            switch (StateNameToEnum(nextState)) {
            case GameStateType::pause_menu:
                return Transition::Push(std::make_shared<PauseMenu>());
            case GameStateType::battle:
                return Transition::Push(std::make_shared<Battle>(this->currentMapEncounterArea));
            default:
                return Transition::None();
            }
        }

        void Map::UpdateMovementInput() {
            const bool* keyboardState = SDL_GetKeyboardState(nullptr);

            if (keyboardState[SDL_SCANCODE_UP]) {
                this->movementInputHeld = true;
                this->movementInputHeldDirection = Scene::Actor::Direction::Up;
            } else if (keyboardState[SDL_SCANCODE_RIGHT]) {
                this->movementInputHeld = true;
                this->movementInputHeldDirection = Scene::Actor::Direction::Right;
            } else if (keyboardState[SDL_SCANCODE_DOWN]) {
                this->movementInputHeld = true;
                this->movementInputHeldDirection = Scene::Actor::Direction::Down;
            } else if (keyboardState[SDL_SCANCODE_LEFT]) {
                this->movementInputHeld = true;
                this->movementInputHeldDirection = Scene::Actor::Direction::Left;
            } else {
                this->movementInputHeld = false;
            }
        }

        std::string Map::ProcessInput(const InputKey key) {
            //std::string result = (*this->luaState.get())["process_input"](static_cast<int>(key));

            return "";
        }

        void Map::ProcessButtonDown(const InputKey key) {
            // Saving this method since it might be useful for other things later.
        }

        void Map::ProcessButtonUp(const InputKey key) {
            // Saving this method since it might be useful for other things later.
        }

        void Map::Render() {
            this->currentMap->Render(this->camera->x, this->camera->y);

            for (auto actor = this->actors.begin(); actor != this->actors.end(); actor++) {
                (*actor)->Render(this->camera);
            }
        }

        void Map::PlaceActor(std::shared_ptr<Scene::Actor> actor, const int x, const int y, const Scene::Actor::Direction direction) {
            actor->currentMap = this->currentMap;
            actor->SetPosition(x, y);
            actor->SetAnimation(Scene::Actor::Animation::Stand);
            actor->SetDirection(direction);
            actor->animationFrame = 0;
            actor->timeSinceLastAnimationFrame = 0.0f;
        }

        void Map::Move(Scene::Actor* actor, const Scene::Actor::Direction direction, const int distance) {
            // Eventually this do pathfinding to enqueue the steps. For now this just enqueues distance number
            // of steps in the direction specificed. Eventually there will probably be an overloaded version
            // of this function that takes a target tile and does pathfinding to get there and enqueues the
            // necessary steps to get there.
            for (int step = 0; step < distance; step++) {
                this->logger->debug() << "Enqueuing step for " << actor->name << " in direction " << static_cast<int>(direction) << ".";

                actor->QueueStep(direction);
            }
        }

        bool Map::TryMove(Scene::Actor* actor, const Scene::Actor::Direction direction) {
            int targetX = actor->GetCurrentTileX();
            int targetY = actor->GetCurrentTileY();

            switch (direction) {
            case Scene::Actor::Direction::Up:
                targetY--;

                break;
            case Scene::Actor::Direction::Right:
                targetX++;

                break;
            case Scene::Actor::Direction::Down:
                targetY++;

                break;
            case Scene::Actor::Direction::Left:
                targetX--;

                break;
            }

            if (targetX < 0 || targetX >= this->currentMap->width || targetY < 0 || targetY >= this->currentMap->height) {
                return false;
            }

            if (!this->currentMap->GetWalkability(targetX, targetY)) {
                return false;
            }

            if (this->IsTileOccupied(targetX, targetY, actor)) {
                return false;
            }

            return true;
        }

        bool Map::IsTileOccupied(const int x, const int y, const Scene::Actor* ignore) const {
            for (auto& actor : actors) {
                if (actor.get() == ignore) {
                    continue;
                }

                if (actor->OccupiesTile(x, y)) {
                    return true;
                }
            }

            return false;
        }

        bool Map::LoadMap(const std::string& mapName, const int startX, const int startY) {
            Services::Locator::WorldService()->GetWorld()->LoadMap(mapName);

            this->currentMap = Services::Locator::WorldService()->GetWorld()->currentMap;
            
            Services::Locator::WorldService()->UpdatePlayerPosition(startX, startY);

            this->PlaceActor(this->player, startX, startY, Scene::Actor::Direction::Down);

            this->camera->Follow(this->player);

            (*this->luaState.get())["after_map_load"]();

            return true;
        }

        bool Map::UnloadMap() {
            Services::Locator::WorldService()->GetWorld()->UnloadMap();

            this->currentMap = Services::Locator::WorldService()->GetWorld()->currentMap;

            if (this->currentMap) {
                return false;
            }

            (*this->luaState.get())["after_map_load"]();

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
            Services::Locator::WorldService()->UpdatePlayerPosition(x, y);

            auto objects = this->currentMap->GetObjects(x, y);

            for (auto object = objects.begin(); object != objects.end(); object++) {
                Objects::Maps::MapLoadPoint* mapLoadPoint = dynamic_cast<Objects::Maps::MapLoadPoint*>(object->get());

                if (mapLoadPoint) {
                    const int startX = mapLoadPoint->GetProperty("x") != "" ? std::stoi(mapLoadPoint->GetProperty("x")) : 0;
                    const int startY = mapLoadPoint->GetProperty("y") != "" ? std::stoi(mapLoadPoint->GetProperty("y")) : 0;

                    this->LoadMap(mapLoadPoint->GetProperty("map"), startX, startY);
                }
            }
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

            // TODO: Handle errors?
            (*this->luaState.get())["initialize"]();
        }

        void Map::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
            sol::table states = (*luaState.get())["states"].get_or_create<sol::table>(sol::new_table());

            states.new_usertype<Map>("Map",
                                     sol::no_constructor,
                                     "pop", &Map::Pop,
                                     "process_input", static_cast<std::string (Map::*)(const InputKey)>(&Map::ProcessInput),
                                     "getCurrentMap", &Map::GetCurrentMap,
                                     "getCurrentMapEncounterArea", &Map::GetCurrentMapEncounterArea,
                                     "setCurrentMapEncounterArea", &Map::SetCurrentMapEncounterArea,
                                     "loadMap", &Map::LoadMap,
                                     "render", &Map::Render,
                                     "step", &Map::Step
                                     );
        }
    }
}
