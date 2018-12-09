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

            Objects::Text::LuaInterface::Bind(this->luaContext);
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

            lua_getglobal(this->luaState, "update");

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

            //        const char LuaMapState::className[] = "MapState";
            //
            //        Lunar<LuaMapState>::RegType LuaMapState::methods[] = {
            //            {"pop", &LuaMapState::pop},
            //            {"getTexture", &LuaMapState::getTexture},
            //            {"getSound", &LuaMapState::getSound},
            //            {"loadMap", &LuaMapState::loadMap},
            //            {"getCurrentMap", &LuaMapState::getCurrentMap},
            //            {"loadTexture", &LuaMapState::loadTexture},
            //            {"setCurrentEncounterArea", &LuaMapState::setCurrentEncounterArea},
            //            {"getPlayerSpriteName", &LuaMapState::getPlayerSpriteName},
            //            {0, 0}
            //        };

            //class LuaMapState : public LuaGameState {
            //public:
            //    static const char className[];
            //    static Lunar<LuaMapState>::RegType methods[];
            //
            //    LuaMapState(lua_State *L) : LuaGameState(L) {
            //        this->realObject = (MapState*)lua_touserdata(L, 1);
            //    }
            //
            //    ~LuaMapState() {
            //
            //    }
            //
            //    void setObject(lua_State *L) {
            //        this->realObject = (MapState*)lua_touserdata(L, 1);
            //    }
            //
            //    int pop(lua_State *L) {
            //        this->realObject->pop = true;
            //
            //        return 0;
            //    }
            //
            //    int getTexture(lua_State *L) {
            //        std::string textureName = luaL_checkstring(L, 1);
            //
            //        GameTexture* texture = this->realObject->textures[textureName];
            //
            //        lua_pushlightuserdata(L, (void*)texture);
            //
            //        return 1;
            //    }
            //
            //    int getCurrentMap(lua_State *L) {
            //        lua_pushlightuserdata(L, (void*)this->realObject->currentMap);
            //
            //        return 1;
            //    }
            //
            //    int loadMap(lua_State *L) {
            //        std::string filename = luaL_checkstring(L, 1);
            //
            //        bool result = this->realObject->LoadMap(filename);
            //
            //        lua_pushboolean(L, result);
            //
            //        return 1;
            //    }
            //
            //    int setCurrentEncounterArea(lua_State *L) {
            //        this->realObject->currentEncounterArea = (GameMapEncounterArea*)lua_touserdata(L, 1);
            //
            //        return 1;
            //    }
            //
            //    int getPlayerSpriteName(lua_State *L) {
            //        //std::shared_ptr<GameTexture> playerSprite = GameState::world->playerParty->characters[0]->spritesheet;
            //
            //        std::string playerSpriteName = GameState::world->playerParty->characters[0]->spritesheetName;
            //
            //        lua_pushstring(L, playerSpriteName.c_str());
            //
            ////        if (playerSprite == nullptr) {
            ////            throw;
            ////        }
            ////
            ////        if (playerSprite) {
            ////            lua_pushlightuserdata(L, static_cast<void*>(playerSprite.get()));
            ////        } else {
            ////            throw;
            ////        }
            //
            //        return 1;
            //    }
            //
            //private:
            //    MapState* realObject;
            //};s
        }
    }
}
