#include "map_tile.hpp"

namespace Game {
    namespace Objects {
        namespace Maps {
            MapTile::MapTile() {
                this->id = -1;
                this->filename = "";
                this->name = "";
                this->width = 0;
                this->height = 0;
                this->texture = nullptr;
            }

            MapTile::~MapTile() {
            }
        }
    }
}
