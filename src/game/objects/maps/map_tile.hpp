#ifndef SDL03_Game_Objects_Maps_MapTile
#define SDL03_Game_Objects_Maps_MapTile

#include <string>

#include "../../assets/texture.hpp"
#include "../base.hpp"

namespace Game {
    namespace Objects {
        namespace Maps {
            class MapTile: public Objects::Base {
            public:
                int id;
                std::string filename;
                std::string name;
                unsigned int width;
                unsigned int height;
                std::shared_ptr<Assets::Texture> texture;

                MapTile();
                ~MapTile();
            };
        }
    }
}

#endif
