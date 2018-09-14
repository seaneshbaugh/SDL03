#ifndef __SDL03__LoggerService__
#define __SDL03__LoggerService__

#include "LoggerCpp.h"

namespace Services {
    namespace Interfaces {
        class LoggerService {
        public:
            ~LoggerService() {}
            virtual Log::Logger GetLogger(std::string channelName) = 0;
        };
    }
}

#endif
