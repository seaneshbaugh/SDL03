#include "GameSettings.h"

GameSettings::GameSettings(std::string applicationName) {
    this->applicationName = applicationName;

    this->settingsDB = nullptr;
}

GameSettings::~GameSettings() {
    if (this->settingsDB) {
        std::cout << "Closing settings database." << std::endl;

        sqlite3_close(this->settingsDB);
    }
}

bool GameSettings::LoadSettings() {
    this->settingsDirectoryPath = FileSystemHelpers::GetApplicationDataDirectory() + "/" + this->applicationName;

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
std::map<int, int> GameSettings::InputSettings() {
    std::map<int, int> invertedInputSettings;

    for (std::map<int, int>::iterator key = this->inputSettings.begin(); key != this->inputSettings.end(); key++) {
        invertedInputSettings[key->second] = key->first;
    }

    return invertedInputSettings;
}

bool GameSettings::CreateSettingsDirectory() {
    return FileSystemHelpers::CreateDirectory(this->settingsDirectoryPath, 0755, true);
}

bool GameSettings::OpenSettingsDatabase() {
    std::cout << "Opening settings database." << std::endl;

    if (sqlite3_open_v2(this->settingsFilePath.c_str(), &this->settingsDB, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, nullptr)) {
        std::cerr << "Error: Could not open settings database file. Got error message: \"" << sqlite3_errmsg(this->settingsDB) << "\"" << std::endl;

        sqlite3_close(this->settingsDB);

        return false;
    }

    return true;
}

bool GameSettings::CreateInputSettingsTable() {
    auto createInputSettingsTableCallback = [] (void* notUsed, int argc, char** argv, char** columnName) {
        return 0;
    };

    char *errorMessage = nullptr;

    if (sqlite3_exec(this->settingsDB, "CREATE TABLE IF NOT EXISTS input_settings(key INTEGER PRIMARY KEY, value INTEGER)", createInputSettingsTableCallback, 0, &errorMessage) != SQLITE_OK) {
        std::cerr << "Error: Could not create input_settings table. Got error message: \"" << errorMessage << "\"" << std::endl;

        sqlite3_free(errorMessage);

        return false;
    }

    return true;
}

bool GameSettings::LoadInputSettings() {
    std::map<int, int> inputSettings;

    auto loadInputSettingsCallback = [] (void* params, int argc, char** argv, char** columnName) {
        std::map<int, int> *inputSettings = static_cast<std::map<int, int>*>(params);

        if (argc == 2) {
            (*inputSettings)[atoi(argv[0])] = atoi(argv[1]);
        } else {
            std::cout << "Warning: expected query to return 2 columns but got " << argc << " instead." << std::endl;
        }

        return 0;
    };

    char *errorMessage = nullptr;

    if (sqlite3_exec(this->settingsDB, "SELECT * FROM input_settings", loadInputSettingsCallback, static_cast<void*>(&inputSettings), &errorMessage) != SQLITE_OK) {
        std::cerr << "SQL error: " << errorMessage << std::endl;

        sqlite3_free(errorMessage);

        return false;
    }

    this->inputSettings = inputSettings;

    return true;
}

bool GameSettings::SaveInputSettings() {
    std::stringstream query;

    query << "INSERT OR REPLACE INTO 'input_settings' ('key', 'value') VALUES";

    std::map<int, int>::iterator lastSetting = this->inputSettings.end();

    --lastSetting;

    for (std::map<int, int>::iterator setting = this->inputSettings.begin(); setting != this->inputSettings.end(); setting++) {
        query << " (" << setting->first << ", " << setting->second << ")";

        if (setting != lastSetting) {
            query << ",";
        }
    }

    query << ";";

    auto saveInputSettingsCallback = [] (void* notUsed, int argc, char** argv, char** columnName) {
        return 0;
    };

    char *errorMessage = nullptr;

    if (sqlite3_exec(this->settingsDB, query.str().c_str(), saveInputSettingsCallback, 0, &errorMessage) != SQLITE_OK) {
        std::cerr << "SQL error: " << errorMessage << std::endl;

        sqlite3_free(errorMessage);

        return false;
    }

    return true;
}

bool GameSettings::VerifyInputSettings() {
    std::map<int, int> defaultInputSettings = this->DefaultInputSettings();

    this->inputSettings.insert(defaultInputSettings.begin(), defaultInputSettings.end());

    return true;
}

// The input settings are inverted from what's actually used by the input mapper.
// The reason for this is that we want the primary key to be the game relevant key
// (otherwise you could end up with multiple SDL keys mapped to the same game key)
// and it's corresponding value to be the mapped SDL key. When loading the settings
// they'll be flipped around. The input mapper needs them in the opposite order
// because we start with the SDL key (from SDL itself) and we use that as the key
// for the map.
std::map<int, int> GameSettings::DefaultInputSettings() {
    std::map<int, int> inputMap;

    inputMap[UP_KEY] = SDLK_UP;

    inputMap[DOWN_KEY] = SDLK_DOWN;

    inputMap[LEFT_KEY] = SDLK_LEFT;

    inputMap[RIGHT_KEY] = SDLK_RIGHT;

    inputMap[CONFIRM_KEY] = SDLK_z;

    inputMap[CANCEL_KEY] = SDLK_x;

    inputMap[MENU_KEY] = SDLK_TAB;

    return inputMap;
}
