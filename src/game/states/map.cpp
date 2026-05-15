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

            // TODO: Do not hard code starting position. This is just for testing purposes. Maybe add a
            // "player_start" object type to the map files and use that to determine where the player
            // starts on the map.
            // I'm not sure if the load point's make much sense. It might be better if the map defines
            // where the player starts.
            // However it's possible there might be cases where depending on how a player
            // enters a map they might start in different locations.
            // What I'm doing here is just for starting on the main world map. Eventually the initial map
            // will be something else entirely. But I need to get movement working smoothly first.
            // What I'm going for right now is rendering the player at the top left corner of the map.
            // They should move down to the right diagonally
            // three tiles per second until they hit the edge of the map. This is just to make sure
            // that Camera::Follow is working as expected.

            this->player = std::make_unique<Player>();
            this->player->currentMap = this->currentMap;


            this->player->walkAnimationFrame = 0;
            this->player->timeSinceLastWalkAnimationFrame = 0.0f;
            this->player->moving = false;
            this->player->movementSpeed = 4.0f * static_cast<float>(this->currentMap->tilewidth);
            // Start facing down. This is just for testing purposes. Eventually the player will start facing in a direction based on the map's load point or something like that.
            // TODO: Use enums to represent directions instead of raw integers. This will make the code more readable and less error prone.
            this->player->movementDirection = 3;
            this->player->playerSpriteName = "stand.down";

            this->player->worldX = static_cast<float>(Services::Locator::WorldService()->GetWorld()->playerCurrentX * this->currentMap->tilewidth);
            this->player->worldY = static_cast<float>(Services::Locator::WorldService()->GetWorld()->playerCurrentY * this->currentMap->tileheight);
            this->player->screenX = 0.0f;
            this->player->screenY = static_cast<float>(this->currentMap->tileheight);
            this->camera->Follow(this->player->worldX, this->player->worldY, this->currentMap->width * this->currentMap->tilewidth, this->currentMap->height * this->currentMap->tileheight);
            SDL_Rect playerSpriteRect = Services::Locator::WorldService()->GetWorld()->playerParty->GetLeader()->GetSpriteRect(this->player->playerSpriteName, this->player->walkAnimationFrame);
            float playerSpriteWidth = static_cast<float>(playerSpriteRect.w);
            float playerSpriteHeight = static_cast<float>(playerSpriteRect.h);
            this->player->screenX = this->player->worldX - this->camera->x;
            this->player->screenY = this->player->worldY - this->camera->y - (playerSpriteHeight - static_cast<float>(this->currentMap->tileheight));

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

        std::shared_ptr<Base> Map::Update(const double deltaTime) {
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

            SDL_Rect playerSpriteRect = Services::Locator::WorldService()->GetWorld()->playerParty->GetLeader()->GetSpriteRect(this->player->playerSpriteName, this->player->walkAnimationFrame);

            float playerSpriteWidth = static_cast<float>(playerSpriteRect.w);
            float playerSpriteHeight = static_cast<float>(playerSpriteRect.h);

            this->camera->Follow(this->player->worldX, this->player->worldY, this->currentMap->width * this->currentMap->tilewidth, this->currentMap->height * this->currentMap->tileheight);

            this->player->screenX = this->player->worldX - this->camera->x;
            this->player->screenY = this->player->worldY - this->camera->y - (playerSpriteHeight - static_cast<float>(this->currentMap->tileheight));

            // this->logger->debug() << "worldX: " << this->worldX << ", worldY: " << this->worldY << ", targetTileX: " << this->targetTileX * this->currentMap->tilewidth << ", targetTileY: " << this->targetTileY * this->currentMap->tileheight << ", cameraX: " << this->camera->x << ", cameraY: " << this->camera->y << ", playerScreenX: " << this->playerScreenX << ", playerScreenY: " << this->playerScreenY << ", moving: " << (this->moving ? "true" : "false") << ", movementDirection: " << this->movementDirection << ", playerspriteName : " << this->playerSpriteName;

            if (this->pop) {
                return nullptr;
            }

            switch (StateNameToEnum(nextState)) {
            case GameStateType::pause_menu:
                return std::make_shared<PauseMenu>();
            case GameStateType::battle:
                return std::make_shared<Battle>(this->currentMapEncounterArea);
            default:
                return this->shared_from_this();
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

            Services::Locator::WorldService()->GetWorld()->playerParty->GetLeader()->Render(this->player->playerSpriteName, this->player->walkAnimationFrame, this->player->screenX, this->player->screenY);
        }

        bool Map::LoadMap(const std::string& mapName, const int startX, const int startY) {
            Services::Locator::WorldService()->GetWorld()->LoadMap(mapName);

            this->currentMap = Services::Locator::WorldService()->GetWorld()->currentMap;

            
            Services::Locator::WorldService()->UpdatePlayerPosition(startX, startY);


            // TODO: Obviously this needs to be turned into a function.
            this->player->currentMap = this->currentMap;

            this->player->walkAnimationFrame = 0;
            this->player->timeSinceLastWalkAnimationFrame = 0.0f;
            this->player->moving = false;
            this->player->movementSpeed = 4.0f * static_cast<float>(this->currentMap->tilewidth);
            // Start facing down. This is just for testing purposes. Eventually the player will start facing in a direction based on the map's load point or something like that.
            // TODO: Use enums to represent directions instead of raw integers. This will make the code more readable and less error prone.
            this->player->movementDirection = 3;
            this->player->playerSpriteName = "stand.down";

            this->player->worldX = static_cast<float>(Services::Locator::WorldService()->GetWorld()->playerCurrentX * this->currentMap->tilewidth);
            this->player->worldY = static_cast<float>(Services::Locator::WorldService()->GetWorld()->playerCurrentY * this->currentMap->tileheight);
            this->player->screenX = 0.0f;
            this->player->screenY = static_cast<float>(this->currentMap->tileheight);
            this->camera->Follow(this->player->worldX, this->player->worldY, this->currentMap->width * this->currentMap->tilewidth, this->currentMap->height * this->currentMap->tileheight);
            SDL_Rect playerSpriteRect = Services::Locator::WorldService()->GetWorld()->playerParty->GetLeader()->GetSpriteRect(this->player->playerSpriteName, this->player->walkAnimationFrame);
            float playerSpriteWidth = static_cast<float>(playerSpriteRect.w);
            float playerSpriteHeight = static_cast<float>(playerSpriteRect.h);
            this->player->screenX = this->player->worldX - this->camera->x;
            this->player->screenY = this->player->worldY - this->camera->y - (playerSpriteHeight - static_cast<float>(this->currentMap->tileheight));

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
