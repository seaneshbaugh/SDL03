#ifndef SDL03_Game_Services_Implementations_SaveManager
#define SDL03_Game_Services_Implementations_SaveManager

#include <fstream>

#include "../../../game.hpp"
#include "../locator.hpp"
#include "../interfaces/save_service.hpp"
#include "../../objects/world.hpp"
#include "../../../helpers/file_system.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            class SaveManager : public Interfaces::SaveService {
            public:
                SaveManager();
                ~SaveManager();
                bool SaveGame(const unsigned int slot);

            private:
                static const std::string logChannel;

                std::shared_ptr<Log::Logger> logger;
                std::string saveDirectoryPath;

                std::string GetSaveDirectoryPath();
                bool CreateSaveDirectory();
            };
        }
    }
}

#endif
