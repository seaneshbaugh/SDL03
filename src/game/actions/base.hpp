#ifndef SDL03_Game_Actions_Base
#define SDL03_Game_Actions_Base

#include <string>

namespace Game {
    namespace Actions {
        class Base {
        public:
            virtual ~Base() = default;
            virtual void Start() = 0;
            virtual void Update(float deltaTime) = 0;
            virtual bool IsCompleted() const = 0;
        };
    }
}

#endif
