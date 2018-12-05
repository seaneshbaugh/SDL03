#include "map_tile.hpp"

namespace Game {
    namespace Objects {
        namespace Maps {
            MapTile::MapTile() {
                this->id = -1;
                this->texture = nullptr;
            }

            MapTile::~MapTile() {
            }
        }
    }
}
