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
            this->LoadLuaContext("scripts/states/map.lua");
        }

        Map::~Map() {
        }

        void Map::LoadLuaContext(const std::string& scriptFileName) {
            this->luaContext = std::make_shared<LuaIntf::LuaContext>();

            Objects::Text::LuaInterface::BindOld(this->luaContext);
            Objects::Image::LuaInterface::BindOld(this->luaContext);
            Objects::Maps::Map::LuaInterface::Bind(this->luaContext);
            Objects::Maps::MapObject::LuaInterface::Bind(this->luaContext);
            Map::LuaInterface::Bind(this->luaContext);

            LuaIntf::Lua::setGlobal(this->luaContext->state(), "map_state", this);

            this->logger->debug() << "Loading \"" << scriptFileName << "\".";

            this->luaContext->doFile(scriptFileName.c_str());

            this->logger->debug() << "Loaded \"" << scriptFileName << "\".";

            LuaIntf::LuaRef initialize(this->luaContext->state(), "initialize");

            // TODO: Handle errors?
            initialize();
        }

        std::shared_ptr<Base> Map::Update(const int key) {
            if (static_cast<InputKey>(key) != InputKey::NO_KEY) {
                this->ProcessInput(key);
            }

            LuaIntf::LuaRef update(this->luaContext->state(), "update");

            std::string nextState = update.call<std::string>();

            if (this->pop) {
                return nullptr;
            }

            switch (StateNameToEnum(nextState)) {
                case GameStateType::battle:
                    //return std::make_shared<Battle>(this->currentMapEncounterArea, nullptr);
                    return this->shared_from_this();
                default:
                    return this->shared_from_this();
            }
        }

        std::shared_ptr<Base> Map::Update(const SDL_Event& event) {
            return this->Update(event.key.keysym.sym);
        }

        std::string Map::ProcessInput(const int key) {
            LuaIntf::LuaRef processInput(this->luaContext->state(), "process_input");

            std::string result = processInput.call<std::string>(key);

            return result;
        }

        void Map::Render() {
            LuaIntf::LuaRef render(this->luaContext->state(), "render");

            render();
        }

        bool Map::LoadMap(const std::string& filename) {
            Services::Locator::WorldService()->GetWorld()->LoadMap(filename);

            this->currentMap = Services::Locator::WorldService()->GetWorld()->currentMap;

            LuaIntf::LuaRef afterMapLoad(this->luaContext->state(), "after_map_load");

            afterMapLoad();

            return true;
        }

        bool Map::UnloadMap() {
            Services::Locator::WorldService()->GetWorld()->UnloadMap();

            this->currentMap = Services::Locator::WorldService()->GetWorld()->currentMap;

            if (this->currentMap) {
                return false;
            }

            LuaIntf::LuaRef afterMapLoad(this->luaContext->state(), "after_map_load");

            afterMapLoad();

            return false;
        }

        void Map::LuaInterface::Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext) {
            LuaIntf::LuaBinding(luaContext->state())
            .beginModule("states")
                .beginClass<Map>("Map")
                    .addConstructor(LUA_SP(std::shared_ptr<Map>), LUA_ARGS())
                    .addFunction("pop", &Map::Pop)
                    .addFunction("process_input", static_cast<std::string(Game::States::Map::*)(const int)>(&Map::ProcessInput))
                    .addFunction("getCurrentMap", [](Map* self){ return self->currentMap.get(); })
                    .addFunction("setCurrentEncounterArea", [](Map* self){ return self->currentMapEncounterArea.get(); })
                    .addFunction("getPlayerSpriteName", [](Map* self){ return Services::Locator::WorldService()->GetWorld()->playerParty->characters[0]->spritesheetName; })
                    .addFunction("render", &Map::Render)
                .endClass()
            .endModule();
        }
    }
}
