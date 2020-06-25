#ifndef SDL03_Game_Services_Implementations_LoggerManager
#define SDL03_Game_Services_Implementations_LoggerManager

#include "../interfaces/logger_service.hpp"
#include "../../../helpers/file_system.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            class LoggerManager : public Interfaces::LoggerService {
            public:
                LoggerManager();
                ~LoggerManager();
                std::shared_ptr<Log::Logger> GetLogger(const std::string& channelName);
            };
        }
    }
}

#endif
