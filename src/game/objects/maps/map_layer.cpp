#include "map_layer.hpp"

namespace Game {
    namespace Objects {
        namespace Maps {
            MapLayer::MapLayer() {
                this->name = "";
                this->visible = true;
            }

            MapLayer::~MapLayer() {
            }

            void MapLayer::Render(int xOffset, int yOffset) {
            }
        }
    }
}
