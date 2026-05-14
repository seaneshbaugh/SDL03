#ifndef SDL03_Game_Objects_Maps_MapStartPoint
#define SDL03_Game_Objects_Maps_MapStartPoint

#include "map_object.hpp"

namespace Game {
    namespace Objects {
        namespace Maps {
            class MapStartPoint : public Objects::Maps::MapObject {
            public:
                MapStartPoint();
                ~MapStartPoint();
            };
        }
    }
}

#endif
