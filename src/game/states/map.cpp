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
            this->camera = std::make_unique<Camera>(0.0f, 0.0f, static_cast<float>(Services::Locator::VideoService()->GetScreenWidth()), static_cast<float>(Services::Locator::VideoService()->GetScreenHeight()));
            this->player = std::make_shared<Actor>();
            this->movementInputHeldDirection = Actor::Direction::Down;
            this->movementInputHeld = false;
            this->PlaceActor(this->player, Services::Locator::WorldService()->GetWorld()->playerCurrentX, Services::Locator::WorldService()->GetWorld()->playerCurrentY, Actor::Direction::Down);
            this->camera->Follow(this->player);
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

            this->player->Update(deltaTime);

            if (this->player->ConsumeCompletedStep()) {
                Services::Locator::WorldService()->UpdatePlayerPosition(this->player->targetTileX, this->player->targetTileY);

                this->logger->debug() << "Player completed a step. New position: (" << this->player->targetTileX << ", " << this->player->targetTileY << ")";

                this->Step(this->player->targetTileX, this->player->targetTileY);
            }

            if (!this->player->moving && this->movementInputHeld) {
                this->player->BeginMovement(Services::Locator::WorldService()->GetWorld()->playerCurrentX, Services::Locator::WorldService()->GetWorld()->playerCurrentY, this-> movementInputHeldDirection);
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
                this->movementInputHeldDirection = Actor::Direction::Up;
            } else if (keyboardState[SDL_SCANCODE_RIGHT]) {
                this->movementInputHeld = true;
                this->movementInputHeldDirection = Actor::Direction::Right;
            } else if (keyboardState[SDL_SCANCODE_DOWN]) {
                this->movementInputHeld = true;
                this->movementInputHeldDirection = Actor::Direction::Down;
            } else if (keyboardState[SDL_SCANCODE_LEFT]) {
                this->movementInputHeld = true;
                this->movementInputHeldDirection = Actor::Direction::Left;
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

            SDL_Rect playerSpriteRect = Services::Locator::WorldService()->GetWorld()->playerParty->GetLeader()->GetSpriteRect(this->player->spriteName, this->player->animationFrame);
            float playerSpriteWidth = static_cast<float>(playerSpriteRect.w);
            float playerSpriteHeight = static_cast<float>(playerSpriteRect.h);
            float playerScreenX = this->player->worldX - this->camera->x;
            float playerScreenY = this->player->worldY - this->camera->y - (playerSpriteHeight - static_cast<float>(this->currentMap->tileheight));

            Services::Locator::WorldService()->GetWorld()->playerParty->GetLeader()->Render(this->player->spriteName, this->player->animationFrame, playerScreenX, playerScreenY);
        }

        void Map::PlaceActor(std::shared_ptr<Actor> actor, const int x, const int y, const Actor::Direction direction) {
            actor->currentMap = this->currentMap;
            actor->animationFrame = 0;
            actor->timeSinceLastAnimationFrame = 0.0f;
            actor->moving = false;
            actor->movementSpeed = 4.0f * static_cast<float>(this->currentMap->tilewidth);
            actor->SetAnimation(Actor::Animation::Stand);
            actor->SetDirection(direction);
            actor->worldX = static_cast<float>(x * this->currentMap->tilewidth);
            actor->worldY = static_cast<float>(y * this->currentMap->tileheight);
        }

        bool Map::LoadMap(const std::string& mapName, const int startX, const int startY) {
            Services::Locator::WorldService()->GetWorld()->LoadMap(mapName);

            this->currentMap = Services::Locator::WorldService()->GetWorld()->currentMap;
            
            Services::Locator::WorldService()->UpdatePlayerPosition(startX, startY);

            this->PlaceActor(this->player, startX, startY, Actor::Direction::Down);

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

        std::string Map::GetPlayerSpriteName() {
            return Services::Locator::WorldService()->GetWorld()->playerParty->characters[0]->spritesheetName;
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
                                     "getPlayerSpriteName", &Map::GetPlayerSpriteName,
                                     "loadMap", &Map::LoadMap,
                                     "render", &Map::Render,
                                     "step", &Map::Step
                                     );
        }
    }
}
