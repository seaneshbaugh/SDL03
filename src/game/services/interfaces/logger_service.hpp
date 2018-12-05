#ifndef SDL03_Game_Services_Interfaces_LoggerService
#define SDL03_Game_Services_Interfaces_LoggerService

#include <memory>

#include "LoggerCpp.h"

namespace Game {
    namespace Services {
        namespace Interfaces {
            class LoggerService {
            public:
                ~LoggerService() {}
                virtual std::shared_ptr<Log::Logger> GetLogger(const std::string& channelName) = 0;
            };
        }
    }
}

#endif
