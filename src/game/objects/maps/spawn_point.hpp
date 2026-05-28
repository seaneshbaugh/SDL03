#ifndef SDL03_Game_Objects_Maps_SpawnPoint
#define SDL03_Game_Objects_Maps_SpawnPoint

#include "map_object.hpp"

namespace Game {
    namespace Objects {
        namespace Maps {
            class SpawnPoint : public MapObject {
            public:
                std::string name;

                SpawnPoint();
                ~SpawnPoint();
            };
        }
    }
}

#endif
