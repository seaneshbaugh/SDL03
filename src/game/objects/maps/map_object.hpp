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
                std::string type;
                std::map<std::string, std::string> properties;

                MapObject();
                ~MapObject();
                SDL_Rect GetPosition();
                void SetPosition(int x, int y);

                class LuaInterface {
                public:
                    static void Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext);
                };
            };
        }
    }
}

#endif
