#include "save_manager.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            const std::string SaveManager::logChannel = "save";

            SaveManager::SaveManager() {
                this->logger = Locator::LoggerService()->GetLogger(SaveManager::logChannel);
                this->saveDirectoryPath = this->GetSaveDirectoryPath();

                this->CreateSaveDirectory();
            }

            SaveManager::~SaveManager() {
            }

            bool SaveManager::SaveGame(const unsigned int slot) {
                // This is a placeholder. I'm not sure if I want this to be mult-
                // threaded or not. I suspect this will take only a few microseconds.

                this->logger->debug() << "Saving game in slot " << slot << ".";

                std::string saveFilePath = this->saveDirectoryPath + "/" + std::to_string(slot) + ".sav";

                std::ofstream saveFile(saveFilePath);

                // TODO: Put this somewhere else...
                json saveData;

                saveData["frameCount"] = Services::Locator::TimeService()->GetFrameCount();
                saveData["world"] = Services::Locator::WorldService()->GetWorld()->AsJSON();

                saveFile << saveData.dump();

                saveFile.close();

                return true;
            }

            std::string SaveManager::GetSaveDirectoryPath() {
                return Helpers::FileSystem::GetApplicationDataDirectory() + "/" + Game::APPLICATION_NAME + "/saves";
            }

            bool SaveManager::CreateSaveDirectory() {
                return Helpers::FileSystem::CreateDirectory(this->saveDirectoryPath, 0755, true);
            }
        }
    }
}
