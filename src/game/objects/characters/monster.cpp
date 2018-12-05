#include "monster.hpp"

namespace Game {
    namespace Objects {
        namespace Characters {
            Monster::Monster() : Base() {
            }

            Monster::~Monster() {
            }

            void Monster::LuaInterface::Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext) {
                //            const char LuaGameMonster::className[] = "GameMonster";
                //
                //            Lunar<LuaGameMonster>::RegType LuaGameMonster::methods[] = {
                //                {"getName", &LuaGameMonster::getName},
                //                {"setName", &LuaGameMonster::setName},
                //                {"getLevel", &LuaGameMonster::getLevel},
                //                {"setLevel", &LuaGameMonster::setLevel},
                //                {"getCurrentHitPoints", &LuaGameMonster::getCurrentHitPoints},
                //                {"setCurrentHitPoints", &LuaGameMonster::setCurrentHitPoints},
                //                {"getMaxHitPoints", &LuaGameMonster::getMaxHitPoints},
                //                {"setMaxHitPoints", &LuaGameMonster::setMaxHitPoints},
                //                {"getCurrentMagicPoints", &LuaGameMonster::getCurrentMagicPoints},
                //                {"setCurrentMagicPoints", &LuaGameMonster::setCurrentMagicPoints},
                //                {"getMaxMagicPoints", &LuaGameMonster::getMaxMagicPoints},
                //                {"setMaxMagicPoints", &LuaGameMonster::setMaxMagicPoints},
                //                {"getStrength", &LuaGameMonster::getStrength},
                //                {"getDexterity", &LuaGameMonster::getDexterity},
                //                {"getIntelligence", &LuaGameMonster::getIntelligence},
                //                {"atbStart", &LuaGameCharacter::atbStart},
                //                {"damage", &LuaGameMonster::damage},
                //                {"render", &LuaGameMonster::render},
                //                {0, 0}
                //            };

                //class LuaGameMonster : public LuaGameCharacter {
                //public:
                //    static const char className[];
                //    static Lunar<LuaGameMonster>::RegType methods[];
                //
                //    LuaGameMonster(lua_State *L) : LuaGameCharacter(L) {
                //        int argc = lua_gettop(L);
                //
                //        if (argc == 1) {
                //            this->realObject = (GameMonster*)lua_touserdata(L, 1);
                //        } else {
                //            this->realObject = new GameMonster();
                //        }
                //    }
                //
                //    ~LuaGameMonster() {
                //        delete this->realObject;
                //    }
                //
                //    int getName(lua_State* L) {
                //        lua_pushstring(L, this->realObject->name.c_str());
                //
                //        return 1;
                //    }
                //
                //    int render(lua_State *L) {
                //        this->realObject->Render(static_cast<int>(luaL_checkinteger(L, 1)), static_cast<int>(luaL_checkinteger(L, 2)));
                //
                //        return 0;
                //    }
                //
                //private:
                //    GameMonster* realObject;
                //};
            }
        }
    }
}
