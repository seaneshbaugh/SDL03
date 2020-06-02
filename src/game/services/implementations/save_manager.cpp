#include "save_manager.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            const std::string SaveManager::logChannel = "save";

            SaveManager::SaveManager() {
                this->logger = Locator::LoggerService()->GetLogger(SaveManager::logChannel);
            }

            SaveManager::~SaveManager() {
            }

            void SaveManager::SaveGame(const unsigned int slot) {
                this->logger->debug() << "Save!";
            }
        }
    }
}
