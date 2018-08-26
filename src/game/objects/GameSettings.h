#ifndef __SDL03__GameSettings__
#define __SDL03__GameSettings__

#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "LoggerCpp.h"
#include "SDL.h"
#include "sqlite3.h"

#include "FileSystemHelpers.h"

#include "GameInputMapper.h"

class GameSettings {
public:
    GameSettings(std::string applicationName);
    ~GameSettings();
    bool LoadSettings();
    std::map<int, int> InputSettings();
private:
    sqlite3 *settingsDB;
    std::string applicationName;
    std::string settingsDirectoryPath;
    std::string settingsFilePath;
    std::map<int, int> inputSettings;
    Log::Logger *logger;

    std::string GetSettingsDirectoryPath();
    bool CreateSettingsDirectory();

    bool OpenSettingsDatabase();

    bool CreateInputSettingsTable();
    bool LoadInputSettings();
    bool SaveInputSettings();
    bool VerifyInputSettings();

    std::map<int, int> DefaultInputSettings();
};

#endif
