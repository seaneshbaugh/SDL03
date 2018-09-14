#include "LoggerManager.h"

namespace Services {
    namespace Implementations {
        LoggerManager::LoggerManager() {
        }

        LoggerManager::~LoggerManager() {
        }

        Log::Logger LoggerManager::GetLogger(std::string channelName) {
            return Log::Logger(channelName.c_str());
        }
    }
}
