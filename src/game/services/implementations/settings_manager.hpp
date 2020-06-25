#ifndef SDL03_Game_Services_Implementations_SettingsManager
#define SDL03_Game_Services_Implementations_SettingsManager

#include <map>

#include <SDL.h>

#include "../../../../lib/sqlite/sqlite3.h"

#include "../locator.hpp"
#include "../interfaces/settings_service.hpp"
#include "../../../helpers/file_system.hpp"

namespace Game {
    namespace Services {
        class Locator;

        namespace Implementations {
            class SettingsManager : public Interfaces::SettingsService {
            public:
                SettingsManager(const std::string& applicationName);
                ~SettingsManager();
                std::map<int, InputKey> InputSettings();

            private:
                static const std::string logChannel;
                static const std::map<InputKey, int> defaultInputSettings;

                std::shared_ptr<Log::Logger> logger;
                sqlite3* settingsDB;
                std::string applicationName;
                std::string settingsDirectoryPath;
                std::string settingsFilePath;
                std::map<InputKey, int> inputSettings;

                bool LoadSettings();
                std::string GetSettingsDirectoryPath();
                bool CreateSettingsDirectory();
                bool OpenSettingsDatabase();
                bool CreateInputSettingsTable();
                bool LoadInputSettings();
                bool SaveInputSettings();
                bool VerifyInputSettings();
            };
        }
    }
}

#endif
