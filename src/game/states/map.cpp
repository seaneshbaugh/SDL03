#include "../objects/world.hpp"
#include "map.hpp"

namespace Game {
    namespace States {
        const std::string Map::logChannel = "states.map";

        Map::Map() {
            this->logger = Services::Locator::LoggerService()->GetLogger(Map::logChannel);
            this->pop = false;
            this->acceptRawInput = false;
            this->currentMap = Services::Locator::WorldService()->GetWorld()->currentMap;
            this->currentMapEncounterArea = nullptr;
            this->LoadLuaState("scripts/states/map.lua");
        }

        Map::~Map() {
        }

        std::shared_ptr<Base> Map::Update(const int key) {
            if (static_cast<InputKey>(key) != InputKey::NO_KEY) {
                this->ProcessInput(key);
            }

            std::string nextState = (*this->luaState.get())["update"]();

            if (this->pop) {
                return nullptr;
            }

            switch (StateNameToEnum(nextState)) {
                case GameStateType::battle:
                    return std::make_shared<Battle>(this->currentMapEncounterArea);
                default:
                    return this->shared_from_this();
            }
        }

        std::shared_ptr<Base> Map::Update(const SDL_Event& event) {
            return this->Update(event.key.keysym.sym);
        }

        std::string Map::ProcessInput(const int key) {
            std::string result = (*this->luaState.get())["process_input"](key);

            return result;
        }

        void Map::Render() {
            (*this->luaState.get())["render"]();
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

        // TODO: Remove this once Base state class no longer defines it.
        void Map::LoadLuaContext(const std::string& scriptFilePath) {
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
                                     "process_input", static_cast<std::string (Map::*)(const int)>(&Map::ProcessInput),
                                     "getCurrentMap", &Map::GetCurrentMap,
                                     "getCurrentMapEncounterArea", &Map::GetCurrentMapEncounterArea,
                                     "setCurrentMapEncounterArea", &Map::SetCurrentMapEncounterArea,
                                     "getPlayerSpriteName", &Map::GetPlayerSpriteName,
                                     "loadMap", &Map::LoadMap,
                                     "render", &Map::Render
                                     );
        }
    }
}
