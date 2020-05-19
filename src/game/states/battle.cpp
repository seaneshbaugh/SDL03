#include "../objects/world.hpp"
#include "battle.hpp"

namespace Game {
    namespace States {
        const std::string Battle::logChannel = "states.battle";

        Battle::Battle(std::shared_ptr<Objects::Maps::MapEncounterArea> encounterArea, std::function<void(Base)> callback) {
            this->logger = Services::Locator::LoggerService()->GetLogger(Battle::logChannel);
            this->pop = false;
            this->acceptRawInput = false;
            // This loads all background textures (well, just one for now, but if there were more
            // they would be loaded. In the future they should be removed from the texture list
            // and loaded only based on what's in the encounter area.
            this->LoadResources("resources/asset_lists/battle_textures.json", "resources/asset_lists/battle_sounds.json");

            this->backgroundName = "battle.background";

            this->background = Services::Locator::TextureService()->AddTexture(this->backgroundName, encounterArea->GetProperty("background"));

            Services::Locator::WorldService()->GetWorld()->SetEnemyParty(encounterArea);

            this->LoadLuaContext("scripts/states/battle.lua");
        }

        Battle::~Battle() {
            // TODO: CLEAR MONSTERS FROM WORLD VIA WORLD SERVICE!!!!!!
        }

        // TODO: Refactor as much of this as possible into base class.
        void Battle::LoadLuaContext(const std::string& scriptFileName) {
            this->luaContext = std::make_shared<LuaIntf::LuaContext>();

            Objects::Text::LuaInterface::BindOld(this->luaContext);
            Objects::Image::LuaInterface::BindOld(this->luaContext);
            Objects::Characters::Base::LuaInterface::Bind(this->luaContext);
            Objects::Characters::Monster::LuaInterface::Bind(this->luaContext);
            Battle::LuaInterface::Bind(this->luaContext);

            LuaIntf::Lua::setGlobal(this->luaContext->state(), "raw_battle_state", this->shared_from_this());

            this->logger->debug() << "Loading \"" << scriptFileName << "\".";

            this->luaContext->doFile(scriptFileName.c_str());

            this->logger->debug() << "Loaded \"" << scriptFileName << "\".";

            LuaIntf::LuaRef initialize(this->luaContext->state(), "initialize");

            // TODO: Handle errors?
            initialize();

            // I don't remember why this is a separate function. There was probably a
            // reason at one point. Probably from when states had callbacks for
            // initialization. This can probably all be moved into the initialize
            // function.
            LuaIntf::LuaRef afterBattleLoad(this->luaContext->state(), "after_battle_load");

            afterBattleLoad();
        }

        std::shared_ptr<Base> Battle::Update(const int key) {
            if (static_cast<InputKey>(key) != InputKey::NO_KEY) {
                this->ProcessInput(key);
            }

            LuaIntf::LuaRef update(this->luaContext->state(), "update");

            std::string nextState = update.call<std::string>();

            if (this->pop) {
                return nullptr;
            }

            return this->shared_from_this();
        }

        std::shared_ptr<Base> Battle::Update(const SDL_Event& event) {
            return this->Update(event.key.keysym.sym);
        }

        std::string Battle::ProcessInput(const int key) {
            LuaIntf::LuaRef processInput(this->luaContext->state(), "process_input");

            std::string result = processInput.call<std::string>(key);

            return result;
        }

        void Battle::Render() {
            LuaIntf::LuaRef render(this->luaContext->state(), "render");

            render();
        }

        void Battle::LuaInterface::Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext) {
            //        const char LuaBattleState::className[] = "BattleState";
            //
            //        Lunar<LuaBattleState>::RegType LuaBattleState::methods[] = {
            //            {"pop", &LuaBattleState::pop},
            //            {"getTexture", &LuaBattleState::getTexture},
            //            {"getSound", &LuaBattleState::getSound},
            //            {"loadTexture", &LuaBattleState::loadTexture},
            //            {"getParty", &LuaBattleState::getParty},
            //            {"getMonsters", &LuaBattleState::getMonsters},
            //            {0, 0}
            //        };

            //class LuaBattleState : public LuaGameState {
            //public:
            //    static const char className[];
            //    static Lunar<LuaBattleState>::RegType methods[];
            //
            //    LuaBattleState(lua_State *L) : LuaGameState(L) {
            //        this->realObject = (BattleState*)lua_touserdata(L, 1);
            //    }
            //
            //    ~LuaBattleState() {
            //
            //    }
            //
            //    void setObject(lua_State *L) {
            //        this->realObject = (BattleState*)lua_touserdata(L, 1);
            //    }
            //
            //    int pop(lua_State *L) {
            //        this->realObject->pop = true;
            //
            //        return 0;
            //    }
            //
            //    int getParty(lua_State *L) {
            //        lua_newtable(L);
            //
            //        int n = 1;
            //
            //        for (std::vector<GameCharacter*>::iterator character = GameState::world->playerParty->characters.begin(); character != GameState::world->playerParty->characters.end(); character++) {
            //            lua_pushlightuserdata(L, (void*)(*character));
            //
            //            lua_rawseti(L, -2, n);
            //
            //            n += 1;
            //        }
            //
            //        return 1;
            //    }
            //
            //    int getMonsters(lua_State *L) {
            //        lua_newtable(L);
            //
            //        int n = 1;
            //
            //        for (std::vector<GameCharacter*>::iterator monster = GameState::world->enemyParty->characters.begin(); monster != GameState::world->enemyParty->characters.end(); monster++) {
            //            lua_pushlightuserdata(L, (void*)(*monster));
            //
            //            lua_rawseti(L, -2, n);
            //
            //            n += 1;
            //        }
            //
            //        return 1;
            //    }
            //
            //private:
            //    BattleState* realObject;
            //};
        }
    }
}
