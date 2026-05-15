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

            this->PlaceActor(this->player, Services::Locator::WorldService()->GetWorld()->playerCurrentX, Services::Locator::WorldService()->GetWorld()->playerCurrentY);

            this->movementInputHeldDirection = 0;
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

            this->player->Update(deltaTime);

            if (!this->player->moving && this->movementInputHeld) {
                auto endMovmentCallback = [this](const int newX, const int newY) {
                    Services::Locator::WorldService()->UpdatePlayerPosition(newX, newY);

                    this->logger->debug() << "Player movement ended. New position: (" << newX << ", " << newY << ")";

                    this->Step(newX, newY);
                };

                this->player->BeginMovement(Services::Locator::WorldService()->GetWorld()->playerCurrentX, Services::Locator::WorldService()->GetWorld()->playerCurrentY, this-> movementInputHeldDirection, endMovmentCallback);
            }

            SDL_Rect playerSpriteRect = Services::Locator::WorldService()->GetWorld()->playerParty->GetLeader()->GetSpriteRect(this->player->spriteName, this->player->walkAnimationFrame);

            float playerSpriteWidth = static_cast<float>(playerSpriteRect.w);
            float playerSpriteHeight = static_cast<float>(playerSpriteRect.h);

            this->camera->Follow(this->player->worldX, this->player->worldY, this->currentMap->width * this->currentMap->tilewidth, this->currentMap->height * this->currentMap->tileheight);

            this->player->screenX = this->player->worldX - this->camera->x;
            this->player->screenY = this->player->worldY - this->camera->y - (playerSpriteHeight - static_cast<float>(this->currentMap->tileheight));

            // this->logger->debug() << "worldX: " << this->worldX << ", worldY: " << this->worldY << ", targetTileX: " << this->targetTileX * this->currentMap->tilewidth << ", targetTileY: " << this->targetTileY * this->currentMap->tileheight << ", cameraX: " << this->camera->x << ", cameraY: " << this->camera->y << ", playerScreenX: " << this->playerScreenX << ", playerScreenY: " << this->playerScreenY << ", moving: " << (this->moving ? "true" : "false") << ", movementDirection: " << this->movementDirection << ", playerspriteName : " << this->spriteName;

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
                this->movementInputHeldDirection = 1; // Up
            } else if (keyboardState[SDL_SCANCODE_RIGHT]) {
                this->movementInputHeld = true;
                this->movementInputHeldDirection = 2; // Right
            } else if (keyboardState[SDL_SCANCODE_DOWN]) {
                this->movementInputHeld = true;
                this->movementInputHeldDirection = 3; // Down
            } else if (keyboardState[SDL_SCANCODE_LEFT]) {
                this->movementInputHeld = true;
                this->movementInputHeldDirection = 4; // Left
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

            Services::Locator::WorldService()->GetWorld()->playerParty->GetLeader()->Render(this->player->spriteName, this->player->walkAnimationFrame, this->player->screenX, this->player->screenY);
        }

        void Map::PlaceActor(std::shared_ptr<Actor> actor, const int x, const int y) {
            actor->currentMap = this->currentMap;

            actor->walkAnimationFrame = 0;
            actor->timeSinceLastWalkAnimationFrame = 0.0f;
            actor->moving = false;
            actor->movementSpeed = 4.0f * static_cast<float>(this->currentMap->tilewidth);
            // Start facing down. This is just for testing purposes. Eventually the player will start facing in a direction based on the map's load point or something like that.
            // TODO: Use enums to represent directions instead of raw integers. This will make the code more readable and less error prone.
            actor->movementDirection = 3;
            actor->spriteName = "stand.down";
            actor->worldX = static_cast<float>(x * this->currentMap->tilewidth);
            actor->worldY = static_cast<float>(y * this->currentMap->tileheight);
            actor->screenX = 0.0f;
            actor->screenY = static_cast<float>(this->currentMap->tileheight);
            this->camera->Follow(actor->worldX, actor->worldY, this->currentMap->width * this->currentMap->tilewidth, this->currentMap->height * this->currentMap->tileheight);
            SDL_Rect actorSpriteRect = Services::Locator::WorldService()->GetWorld()->playerParty->GetLeader()->GetSpriteRect(actor->spriteName, actor->walkAnimationFrame);
            float actorSpriteWidth = static_cast<float>(actorSpriteRect.w);
            float actorSpriteHeight = static_cast<float>(actorSpriteRect.h);
            actor->screenX = actor->worldX - this->camera->x;
            actor->screenY = actor->worldY - this->camera->y - (actorSpriteHeight - static_cast<float>(this->currentMap->tileheight));
        }

        bool Map::LoadMap(const std::string& mapName, const int startX, const int startY) {
            Services::Locator::WorldService()->GetWorld()->LoadMap(mapName);

            this->currentMap = Services::Locator::WorldService()->GetWorld()->currentMap;
            
            Services::Locator::WorldService()->UpdatePlayerPosition(startX, startY);

            this->PlaceActor(this->player, startX, startY);

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

        std::string Map::PlayerSpriteDirection() {
            switch (this->movementDirection) {
            case 1:
                return "up";
            case 2:
                return "right";
            case 3:
                return "down";
            case 4:
                return "left";
            }

            return "down";
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
