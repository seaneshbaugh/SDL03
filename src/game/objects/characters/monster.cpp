#include "monster.hpp"

namespace Game {
    namespace Objects {
        namespace Characters {
            Monster::Monster() : Base() {
            }

            Monster::~Monster() {
            }

            void Monster::Render(const int x, const int y) {
                const SDL_Rect srcrect = {0, 0, 32, 32};

                const SDL_Rect dstrect = {x, y, 32, 32};

                Services::Locator::VideoService()->Render(this->sprite, &srcrect, &dstrect);
            }

            void Monster::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
                sol::table objects = (*luaState.get())["objects"].get_or_create<sol::table>(sol::new_table());

                sol::table characters = objects["characters"].get_or_create<sol::table>(sol::new_table());

                characters.new_usertype<Monster>("Monster",
                                                 sol::constructors<Monster()>(),
                                                 "getName", &Monster::GetName,
                                                 "setName", &Monster::SetName,
                                                 "getLevel", &Monster::GetLevel,
                                                 "setLevel", &Monster::SetLevel,
                                                 "getCurrentHitPoints", &Monster::GetCurrentHitPoints,
                                                 "setCurrentHitPoints", &Monster::SetCurrentHitPoints,
                                                 "getMaxHitPoints", &Monster::GetMaxHitPoints,
                                                 "setMaxHitPoints", &Monster::SetMaxHitPoints,
                                                 "getCurrentMagicPoints", &Monster::GetCurrentMagicPoints,
                                                 "setCurrentMagicPoints", &Monster::SetCurrentMagicPoints,
                                                 "getMaxMagicPoints", &Monster::GetMaxMagicPoints,
                                                 "setMaxMagicPoints", &Monster::SetMaxMagicPoints,
                                                 "getStrength", &Monster::GetStrength,
                                                 "getDexterity", &Monster::GetDexterity,
                                                 "getIntelligence", &Monster::GetIntelligence,
                                                 "atbStart", &Monster::ATBStart,
                                                 "damage", &Monster::Damage,
                                                 "render", &Monster::Render,
                                                 sol::base_classes, sol::bases<Base>()
                                                 );

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
