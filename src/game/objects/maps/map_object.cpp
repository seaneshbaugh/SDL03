#include "map_object.hpp"

namespace Game {
    namespace Objects {
        namespace Maps {
            MapObject::MapObject() {
            }

            MapObject::~MapObject() {
            }

            SDL_Rect MapObject::GetPosition() {
                return SDL_Rect {this->x, this->y, 1, 1};
            }

            void MapObject::SetPosition(int x, int y) {
                this->x = x;
                this->y = y;
            }

            void MapObject::LuaInterface::Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext) {
                //            const char LuaGameMapObject::className[] = "GameMapObject";
                //
                //            Lunar<LuaGameMapObject>::RegType LuaGameMapObject::methods[] = {
                //                {"getX", &LuaGameMapObject::getX},
                //                {"getY", &LuaGameMapObject::getY},
                //                {"setPosition", &LuaGameMapObject::setPosition},
                //                {"type", &LuaGameMapObject::type},
                //                {"getProperty", &LuaGameMapObject::getProperty},
                //                {"setProperty", &LuaGameMapObject::setProperty},
                //                {0, 0}
                //            };
                //class LuaGameMapObject {
                //public:
                //    static const char className[];
                //    static Lunar<LuaGameMapObject>::RegType methods[];
                //
                //    LuaGameMapObject(lua_State *L) {
                //        int argc = lua_gettop(L);
                //
                //        if (argc == 1) {
                //            this->realObject = (GameMapObject*)lua_touserdata(L, 1);
                //
                //            this->createdFromLua = false;
                //        } else {
                //            this->realObject = new GameMapObject();
                //
                //            this->createdFromLua = true;
                //        }
                //    }
                //
                //    ~LuaGameMapObject() {
                //        if (this->createdFromLua) {
                //            delete this->realObject;
                //        }
                //    }
                //
                //    int getX(lua_State *L) {
                //        lua_pushnumber(L, this->realObject->x);
                //
                //        return 1;
                //    }
                //
                //    int getY(lua_State *L) {
                //        lua_pushnumber(L, this->realObject->y);
                //
                //        return 1;
                //    }
                //
                //    int setPosition(lua_State *L) {
                //        int x = (int)luaL_checkinteger(L, 1);
                //
                //        int y = (int)luaL_checkinteger(L, 2);
                //
                //        this->realObject->SetPosition(x, y);
                //
                //        return 0;
                //    }
                //
                //    int type(lua_State *L) {
                //        lua_pushstring(L, this->realObject->type.c_str());
                //
                //        return 1;
                //    }
                //
                //    int getProperty(lua_State *L) {
                //        std::string key = std::string(luaL_checkstring(L, 1));
                //
                //        try {
                //            lua_pushstring(L, this->realObject->properties.at(key).c_str());
                //        } catch (const std::out_of_range& exception) {
                //            lua_pushnil(L);
                //        }
                //
                //        return 1;
                //    }
                //
                //    int setProperty(lua_State *L) {
                //        std::string key = std::string(luaL_checkstring(L, 1));
                //
                //        std::string value = std::string(luaL_checkstring(L, 2));
                //
                //        this->realObject->properties[key] = value;
                //
                //        return 0;
                //    }
                //private:
                //    GameMapObject* realObject;
                //    // Yuck.
                //    bool createdFromLua;
                //};
            }
        }
    }
}
