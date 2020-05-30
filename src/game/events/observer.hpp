#ifndef SDL03_Game_Events_Observer
#define SDL03_Game_Events_Observer

#include "base.hpp"

namespace Game {
    namespace Events {
        class Observer {
            virtual ~Observer();
            virtual void OnNotify(Base event) = 0;
        };
    }
}

#endif
