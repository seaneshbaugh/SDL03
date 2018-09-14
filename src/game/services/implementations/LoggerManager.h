#ifndef __SDL03__LoggerManager__
#define __SDL03__LoggerManager__

#include "../interfaces/LoggerService.h"

namespace Services {
    namespace Implementations {
        class LoggerManager : public Interfaces::LoggerService {
        public:
            LoggerManager();
            ~LoggerManager();
            Log::Logger GetLogger(std::string channelName);
        };
    }
}

#endif
