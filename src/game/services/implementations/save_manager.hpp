#ifndef SDL03_Game_Services_Implementations_SaveManager
#define SDL03_Game_Services_Implementations_SaveManager

#include "../locator.hpp"
#include "../interfaces/save_service.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            class SaveManager : public Interfaces::SaveService {
            public:
                SaveManager();
                ~SaveManager();
                void SaveGame(const unsigned int slot);

            private:
                static const std::string logChannel;

                std::shared_ptr<Log::Logger> logger;
            };
        }
    }
}

#endif
