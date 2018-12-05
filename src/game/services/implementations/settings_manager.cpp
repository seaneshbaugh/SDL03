#include "settings_manager.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            const std::string SettingsManager::logChannel = "settings";

            // The input settings are inverted from what's actually used by the input
            // mapper. The reason for this is that we want the primary key to be the game
            // relevant key (otherwise you could end up with multiple SDL keys mapped to
            // the same game key) and it's corresponding value to be the mapped SDL key.
            // When loading the settings they'll be flipped around. The input mapper
            // needs them in the opposite order because we start with the SDL key (from
            // SDL itself) and we use that as the key for the map.
            const std::map<InputKey, int> SettingsManager::defaultInputSettings {
                {InputKey::UP_KEY, SDLK_UP},
                {InputKey::DOWN_KEY, SDLK_DOWN},
                {InputKey::LEFT_KEY, SDLK_LEFT},
                {InputKey::RIGHT_KEY, SDLK_RIGHT},
                {InputKey::CONFIRM_KEY, SDLK_z},
                {InputKey::CANCEL_KEY, SDLK_x},
                {InputKey::MENU_KEY, SDLK_TAB}
            };

            SettingsManager::SettingsManager(const std::string& applicationName) {
                if (Locator::LoggerService() == nullptr) {
                    throw std::runtime_error("LoggerService must be started before SettingsService.");
                }

                this->logger = Locator::LoggerService()->GetLogger(SettingsManager::logChannel);
                this->applicationName = applicationName;
                this->settingsDB = nullptr;
                this->LoadSettings();
            }

            SettingsManager::~SettingsManager() {
                if (this->settingsDB) {
                    this->logger->info() << "Closing settings database.";

                    sqlite3_close(this->settingsDB);
                }
            }

            bool SettingsManager::LoadSettings() {
                this->settingsDirectoryPath = Helpers::FileSystem::GetApplicationDataDirectory() + "/" + this->applicationName;

                if (!this->CreateSettingsDirectory()) {
                    return false;
                }

                this->settingsFilePath = this->settingsDirectoryPath + "/settings.sqlite3";

                if (!this->OpenSettingsDatabase()) {
                    return false;
                }

                this->CreateInputSettingsTable();
                this->LoadInputSettings();
                this->VerifyInputSettings();
                this->SaveInputSettings();

                return true;
            }

            // See DefaultInputSettings for why this is inverted.
            std::map<int, InputKey> SettingsManager::InputSettings() {
                std::map<int, InputKey> invertedInputSettings;

                for (auto it = this->inputSettings.begin(); it != this->inputSettings.end(); ++it) {
                    invertedInputSettings[it->second] = it->first;
                }

                return invertedInputSettings;
            }

            bool SettingsManager::CreateSettingsDirectory() {
                return Helpers::FileSystem::CreateDirectory(this->settingsDirectoryPath, 0755, true);
            }

            bool SettingsManager::OpenSettingsDatabase() {
                this->logger->info() << "Opening settings database \"" << this->settingsFilePath.c_str() << "\".";

                const int flags = SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE;

                if (sqlite3_open_v2(this->settingsFilePath.c_str(), &this->settingsDB, flags, nullptr)) {
                    this->logger->error() << "Could not open settings database file. " << sqlite3_errmsg(this->settingsDB);

                    sqlite3_close(this->settingsDB);

                    return false;
                }

                return true;
            }

            bool SettingsManager::CreateInputSettingsTable() {
                auto createInputSettingsTableCallback = [] (void* notUsed, int argc, char** argv, char** columnName) {
                    return 0;
                };

                constexpr const char* query = "CREATE TABLE IF NOT EXISTS input_settings(key INTEGER PRIMARY KEY, value INTEGER)";

                char* errorMessage = nullptr;

                if (sqlite3_exec(this->settingsDB, query, createInputSettingsTableCallback, 0, &errorMessage) != SQLITE_OK) {
                    this->logger->error() << "Could not create input_settings table. " << errorMessage;

                    sqlite3_free(errorMessage);

                    return false;
                }

                return true;
            }

            bool SettingsManager::LoadInputSettings() {
                std::map<InputKey, int> inputSettings;

                auto loadInputSettingsCallback = [] (void* params, int argc, char** argv, char** columnName) {
                    std::map<InputKey, int>* inputSettings = static_cast<std::map<InputKey, int>*>(params);

                    if (argc == 2) {
                        (*inputSettings)[InputKey(atoi(argv[0]))] = atoi(argv[1]);
                    } else {
                        // We can't capture this for the auto lambda because sqlite3_exec has no definition which
                        // matches whatever sqlite3_exec
                        auto logger = Services::Locator::LoggerService()->GetLogger(SettingsManager::logChannel);

                        logger->warning() << "expected query to return 2 columns but got " << argc << " instead.";
                    }

                    return 0;
                };

                constexpr const char* query = "SELECT * FROM input_settings";

                char* errorMessage = nullptr;

                if (sqlite3_exec(this->settingsDB, query, loadInputSettingsCallback, static_cast<void*>(&inputSettings), &errorMessage) != SQLITE_OK) {
                    this->logger->error() << "SQL error: " << errorMessage;

                    sqlite3_free(errorMessage);

                    return false;
                }

                this->inputSettings = inputSettings;

                return true;
            }

            bool SettingsManager::SaveInputSettings() {
                std::stringstream query;

                query << "INSERT OR REPLACE INTO 'input_settings' ('key', 'value') VALUES";

                auto lastSetting = this->inputSettings.end();

                --lastSetting;

                for (auto setting = this->inputSettings.begin(); setting != this->inputSettings.end(); setting++) {
                    query << " (" << static_cast<int>(setting->first) << ", " << setting->second << ")";

                    if (setting != lastSetting) {
                        query << ",";
                    }
                }

                query << ";";

                auto saveInputSettingsCallback = [] (void* notUsed, int argc, char** argv, char** columnName) {
                    return 0;
                };

                char* errorMessage = nullptr;

                if (sqlite3_exec(this->settingsDB, query.str().c_str(), saveInputSettingsCallback, 0, &errorMessage) != SQLITE_OK) {
                    this->logger->error() << "SQL error: " << errorMessage;

                    sqlite3_free(errorMessage);

                    return false;
                }

                return true;
            }

            bool SettingsManager::VerifyInputSettings() {
                std::map<InputKey, int> defaultInputSettings = SettingsManager::defaultInputSettings;

                this->inputSettings.insert(defaultInputSettings.begin(), defaultInputSettings.end());

                return true;
            }
        }
    }
}
