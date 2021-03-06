#ifndef SDL03_Game_Objects_Base
#define SDL03_Game_Objects_Base

#include <memory>

#include "../../../lib/LoggerCpp/include/LoggerCpp/LoggerCpp.h"
#ifdef __APPLE__
#include "../../../lib/lua/src/lua.h"
#include "../../../lib/lua/src/lualib.h"
#include "../../../lib/lua/src/lauxlib.h"
#else
#include "../../../lib/lua/src/lua.hpp"
#endif
#include "../../../lib/sol/sol.hpp"

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
