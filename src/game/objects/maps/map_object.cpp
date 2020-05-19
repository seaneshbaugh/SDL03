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

            std::string MapObject::GetType() {
                return this->type;
            }

            void MapObject::SetType(const std::string& type) {
                this->type = type;
            }

            std::string MapObject::GetProperty(const std::string& name) {
                return this->properties.at(name);
            }

            std::string MapObject::SetProperty(const std::string& name, const std::string& value) {
                return this->properties[name] = value;
            }

            std::map<std::string, std::string> MapObject::SetProperties(const std::map<std::string, std::string>& properties) {
                for (auto property = properties.begin(); property != properties.end(); ++property) {
                    this->properties[(*property).first] = (*property).second;
                }

                return this->properties;
            }

            void MapObject::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
                sol::table objects = (*luaState.get())["objects"].get_or_create<sol::table>(sol::new_table());

                objects.new_usertype<MapObject>("MapObject",
                                                sol::constructors<MapObject()>(),
                                                "getX", &MapObject::x,
                                                "getY", &MapObject::y,
                                                "getType", &MapObject::GetType,
                                                "getProperty", &MapObject::GetProperty,
                                                "setProperty", &MapObject::SetProperty
                                                );
            }
        }
    }
}
