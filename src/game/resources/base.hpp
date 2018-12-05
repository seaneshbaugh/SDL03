#ifndef SDL03_Game_Resources_Base
#define SDL03_Game_Resources_Base

#include <memory>
#include <string>

#include "../services/locator.hpp"
#include "LoggerCpp.h"

namespace Game {
    namespace Resources {
        class Base {
        public:
            virtual ~Base() {}
            virtual void Load(const std::string& filename) = 0;

        protected:
            std::shared_ptr<Log::Logger> logger;
        };
    }
}

#endif
