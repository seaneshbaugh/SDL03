#include "map_layer.hpp"

namespace Game {
    namespace Objects {
        namespace Maps {
            const std::map<std::string, MapLayerType> mapLayerTypeMap = {
                {"terrain", MapLayerType::Terrain},
                {"objects", MapLayerType::Objects},
                {"walkability", MapLayerType::Walkability},
                {"effects", MapLayerType::Effects}
            };

            MapLayerType MapLayerNameToEnum(const std::string& mapLayerName) {
                return Maps::mapLayerTypeMap.at(mapLayerName);
            }

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
