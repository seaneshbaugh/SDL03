#ifndef SDL03_Game_MovementCommand
#define SDL03_Game_MovementCommand

#include "actor.hpp"

namespace Game {
    namespace Scene {
        struct MovementCommand {
            Actor::Direction direction;
            int distance;
        };
    }
}

#endif
