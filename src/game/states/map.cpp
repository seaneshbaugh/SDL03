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
            // Get the viewport dimensions from the video service. This is just for testing purposes for now.
            // Eventually the viewport dimensions will be determined by the window size and maybe some sort
            // of settings option for how much of the map to show on screen at once.
            this->camera = std::make_unique<Camera>(0.0f, 0.0f, 640.0f, 480.0f);

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


            this->walkAnimationFrame = 0;
            this->timeSinceLastWalkAnimationFrame = 0.0f;
            this->moving = false;
            this->movementSpeed = 4.0f * static_cast<float>(this->currentMap->tilewidth);
            // Start facing down. This is just for testing purposes. Eventually the player will start facing in a direction based on the map's load point or something like that.
            // TODO: Use enums to represent directions instead of raw integers. This will make the code more readable and less error prone.
            this->movementDirection = 3;
            this->playerSpriteName = "stand.down";

            this->worldX = static_cast<float>(Services::Locator::WorldService()->GetWorld()->playerCurrentX * this->currentMap->tilewidth);
            this->worldY = static_cast<float>(Services::Locator::WorldService()->GetWorld()->playerCurrentY * this->currentMap->tileheight);
            this->playerScreenX = 0.0f;
            this->playerScreenY = static_cast<float>(this->currentMap->tileheight);
            this->camera->Follow(this->worldX, this->worldY, this->currentMap->width * this->currentMap->tilewidth, this->currentMap->height * this->currentMap->tileheight);
            SDL_Rect playerSpriteRect = Services::Locator::WorldService()->GetWorld()->playerParty->GetLeader()->GetSpriteRect(this->playerSpriteName, this->walkAnimationFrame);
            float playerSpriteWidth = static_cast<float>(playerSpriteRect.w);
            float playerSpriteHeight = static_cast<float>(playerSpriteRect.h);
            this->playerScreenX = this->worldX - this->camera->x;
            this->playerScreenY = this->worldY - this->camera->y - (playerSpriteHeight - static_cast<float>(this->currentMap->tileheight));

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

            if (!this->moving && this->movementInputHeld) {
                this->BeginMovement(this->movementInputHeldDirection);
            }

            if (this->moving) {
                switch (this->movementDirection) {
                case 1: // Up
                    this->worldY -= this->movementSpeed * deltaTime;

                    if (this->worldY <= this->targetTileY * this->currentMap->tileheight) {
                        this->worldY = this->targetTileY * this->currentMap->tileheight;

                        this->moving = false;

                        Services::Locator::WorldService()->UpdatePlayerPosition(this->targetTileX, this->targetTileY);
                    }
                    break;
                case 2: // Right
                    this->worldX += this->movementSpeed * deltaTime;

                    if (this->worldX >= this->targetTileX * this->currentMap->tilewidth) {
                        this->worldX = this->targetTileX * this->currentMap->tilewidth;

                        this->moving = false;

                        Services::Locator::WorldService()->UpdatePlayerPosition(this->targetTileX, this->targetTileY);
                    }
                    break;
                case 3: // Down
                    this->worldY += this->movementSpeed * deltaTime;

                    if (this->worldY >= this->targetTileY * this->currentMap->tileheight) {
                        this->worldY = this->targetTileY * this->currentMap->tileheight;

                        this->moving = false;

                        Services::Locator::WorldService()->UpdatePlayerPosition(this->targetTileX, this->targetTileY);
                    }
                    break;
                case 4: // Left
                    this->worldX -= this->movementSpeed * deltaTime;

                    if (this->worldX <= this->targetTileX * this->currentMap->tilewidth) {
                        this->worldX = this->targetTileX * this->currentMap->tilewidth;

                        this->moving = false;

                        Services::Locator::WorldService()->UpdatePlayerPosition(this->targetTileX, this->targetTileY);
                    }
                    break;
                }

                this->playerSpriteName = "walk." + this->PlayerSpriteDirection();

                this->timeSinceLastWalkAnimationFrame += deltaTime;

                // There are 8 frames in the walk animation right now. It's very unlikely that'll ever change, but it'd
                // still be a good idea to not hard code that value here. Maybe add a function to the Character class
                // that returns the number of frames in the walk animation and then use the reciprocal.
                if (this->timeSinceLastWalkAnimationFrame >= 0.125f) {
                    this->walkAnimationFrame = (this->walkAnimationFrame + 1) % 8;
                    this->timeSinceLastWalkAnimationFrame = 0.0f;
                }
            } else {
                // I don't like how I'm resetting this on every frame where the player is standing still. It feels wasteful.
                // But I'm already running at like 2000 FPS on my computer so I don't think it's a big deal. If it becomes
                // a problem then I can always add a separate variable to track the player's current sprite and only update
                // it when the direction changes or something like that.
                this->playerSpriteName = "stand." + this->PlayerSpriteDirection();
                this->walkAnimationFrame = 0;
                this->timeSinceLastWalkAnimationFrame = 0.0f;
            }

            SDL_Rect playerSpriteRect = Services::Locator::WorldService()->GetWorld()->playerParty->GetLeader()->GetSpriteRect(this->playerSpriteName, this->walkAnimationFrame);

            float playerSpriteWidth = static_cast<float>(playerSpriteRect.w);
            float playerSpriteHeight = static_cast<float>(playerSpriteRect.h);

            this->camera->Follow(this->worldX, this->worldY, this->currentMap->width * this->currentMap->tilewidth, this->currentMap->height * this->currentMap->tileheight);

            this->playerScreenX = this->worldX - this->camera->x;
            this->playerScreenY = this->worldY - this->camera->y - (playerSpriteHeight - static_cast<float>(this->currentMap->tileheight));

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

        void Map::BeginMovement(const int direction) {
            this->startTileX = Services::Locator::WorldService()->GetWorld()->playerCurrentX;
            this->startTileY = Services::Locator::WorldService()->GetWorld()->playerCurrentY;

            this->targetTileX = this->startTileX;
            this->targetTileY = this->startTileY;

            switch (direction) {
            case 1: // Up
                // TODO: Add "walkability" layer collision detection here????
                if (this->targetTileY > 0) {
                    this->targetTileY--;
                }

                break;
            case 2: // Right
                if (this->targetTileX < Services::Locator::WorldService()->GetWorld()->currentMap->width - 1) {
                    targetTileX++;
                }

                break;
            case 3: // Down
                if (this->targetTileY < Services::Locator::WorldService()->GetWorld()->currentMap->height - 1) {
                    this->targetTileY++;
                }

                break;
            case 4: // Left
                if (this->targetTileX > 0) {
                    this->targetTileX--;
                }

                break;
            }

            // Only start moving if the target tile is different from the starting tile.
            if ((this->targetTileX != this->startTileX || this->targetTileY != this->startTileY) && this->currentMap->GetWalkability(this->targetTileX, this->targetTileY)) {
                this->moving = true;
            }

            // Always change movement direction so even if we can't move in that direction we still face the correct way.
            this->movementDirection = direction;
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

            Services::Locator::WorldService()->GetWorld()->playerParty->GetLeader()->Render(this->playerSpriteName, this->walkAnimationFrame, this->playerScreenX, this->playerScreenY);
        }

        bool Map::LoadMap(const std::string& filename) {
            Services::Locator::WorldService()->GetWorld()->LoadMap(filename);

            this->currentMap = Services::Locator::WorldService()->GetWorld()->currentMap;

            // The Lua after_map_load function is probably where we want to set the player's starting position and initial
            // movement direction.
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
