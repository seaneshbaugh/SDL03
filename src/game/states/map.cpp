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
            Services::Locator::WorldService()->UpdatePlayerPosition(0, 0);
            this->worldX = 0.0f;
            this->worldY = 0.0f;


            this->player = std::make_unique<Objects::Image>(this->GetPlayerSpriteName(), 320, 224);
        }

        Map::~Map() {
        }

        void Map::HandleEvent(const SDL_Event& event) {
            InputKey key = Services::Locator::InputService()->GetInputMapKey(event);

            if (key != InputKey::NO_KEY) {
                this->ProcessInput(key);
            }
        }

        std::shared_ptr<Base> Map::Update(const double deltaTime) {
            std::string nextState = (*this->luaState.get())["update"](deltaTime);

            float newX = this->worldX + (96 * deltaTime);
            float newY = this->worldX + (96 * deltaTime);

            this->logger->debug() << "newX: " << newX << ", newY: " << newY;

            if (newX >= this->currentMap->width * this->currentMap->tilewidth) {
                newX = this->currentMap->width * this->currentMap->tilewidth;
            }

            if (newY >= this->currentMap->height * this->currentMap->tileheight) {
                newY = this->currentMap->height * this->currentMap->tileheight;
            }

            this->camera->Follow(newX, newY, this->currentMap->width * this->currentMap->tilewidth, this->currentMap->height * this->currentMap->tileheight);

            this->worldX = newX;
            this->worldY = newY;

            float playerScreenX = this->worldX - this->camera->x;
            float playerScreenY = this->worldY - this->camera->y - (48 - this->currentMap->tileheight);

            this->logger->debug() << "playerScreenX: " << playerScreenX << ", playerScreenY: " << playerScreenY;

            if (playerScreenX > this->camera->viewportWidth - 32) {
                playerScreenX = this->camera->viewportWidth - 32;
            }

            if (playerScreenY > this->camera->viewportHeight - 48) {
                playerScreenY = this->camera->viewportHeight - 48;
            }

            this->player->SetPosition(playerScreenX, playerScreenY);



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

        std::string Map::ProcessInput(const InputKey key) {
            std::string result = (*this->luaState.get())["process_input"](static_cast<int>(key));

            return result;
        }

        void Map::Render() {
            // (*this->luaState.get())["render"]();

            //this->currentMap->Render(Services::Locator::WorldService()->GetWorld()->playerCurrentX * this->currentMap->tilewidth, Services::Locator::WorldService()->GetWorld()->playerCurrentY * this->currentMap->tileheight);

            this->currentMap->Render(this->camera->x, this->camera->y);

            this->player->Render(0, 48 * 0, 32, 48);
        }

        bool Map::LoadMap(const std::string& filename) {
            Services::Locator::WorldService()->GetWorld()->LoadMap(filename);

            this->currentMap = Services::Locator::WorldService()->GetWorld()->currentMap;

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
