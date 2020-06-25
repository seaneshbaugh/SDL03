#ifndef SDL03_Game_Objects_Maps_MapLayer
#define SDL03_Game_Objects_Maps_MapLayer

#include <vector>

#include "map_object.hpp"

namespace Game {
    namespace Objects {
        namespace Maps {
            enum class MapLayerType {
                Terrain,
                Objects,
                Walkability,
                Effects
            };

            enum class MapObjectLayerType {
                LoadPoint,
                EncounterArea
            };

            namespace {
                static const std::map<std::string, MapLayerType> mapLayerTypeMap;
            }

            MapLayerType MapLayerNameToEnum(const std::string& mapLayerName);

            class MapLayer : public Objects::Base {
            public:
                std::string name;
                bool visible;
                MapLayerType type;
                std::vector <int> tiles;
                std::vector <std::shared_ptr<MapObject>> objects;

                MapLayer();
                ~MapLayer();
                void Render(int xOffset, int yOffset);
            };
        }
    }
}


#endif
