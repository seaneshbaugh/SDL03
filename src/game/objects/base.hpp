#ifndef SDL03_Game_Objects_Base
#define SDL03_Game_Objects_Base

#include <memory>
#include "LuaIntf.h"

#include "LoggerCpp.h"

namespace Game {
    namespace Objects {
        class Base : public std::enable_shared_from_this<Base> {
        public:
            Base();
            ~Base();

        protected:
            std::shared_ptr<Log::Logger> logger;
        };
    }
}

#endif