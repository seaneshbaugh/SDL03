#ifndef SDL03_Game_Objects_Maps_MapObject
#define SDL03_Game_Objects_Maps_MapObject

#include <map>
#include <string>

#include "SDL.h"

#include "../base.hpp"

namespace Game {
    namespace Objects {
        namespace Maps {
            class MapObject : public Objects::Base {
            public:
                int x;
                int y;
                int width;
                int height;

                MapObject();
                virtual ~MapObject();
                SDL_Rect GetPosition();
                void SetPosition(int x, int y);
                std::string GetType();
                void SetType(const std::string& type);
                std::string const GetProperty(const std::string& name) const;
                std::string SetProperty(const std::string& name, const std::string& value);
                std::map<std::string, std::string> SetProperties(const std::map<std::string, std::string>& properties);

            private:
                std::string type;
                std::map<std::string, std::string> properties;

            public:
                class LuaInterface {
                public:
                    static void Bind(std::shared_ptr<sol::state> luaState);
                };
            };
        }
    }
}

#endif
