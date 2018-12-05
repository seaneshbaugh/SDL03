#include "logger_manager.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            LoggerManager::LoggerManager() {
#ifndef NDEBUG
                Log::Manager::setDefaultLevel(Log::Log::eDebug);
#else
                Log::Manager::setDefaultLevel(Log::Log::eError);
#endif

                const std::string logDirectory = Helpers::FileSystem::GetLogDirectory();
                Log::Config::Vector configList;

                Log::Config::addOutput(configList, "OutputConsole");
                Log::Config::addOutput(configList, "OutputFile");
                Log::Config::setOption(configList, "filename", (logDirectory + "/SDL03.log").c_str());
                Log::Config::setOption(configList, "filename_old", (logDirectory + "/SDL03.old.log").c_str());
                Log::Config::setOption(configList, "max_startup_size", "0");
                Log::Config::setOption(configList, "max_size", "16777216");

                Log::Manager::configure(configList);

                // TODO: Add the ability to set log levels via a config file.
                // TODO: Use class constants for log channel name.
                Log::Manager::get("engine")->setLevel(Log::Log::eError);
                Log::Manager::get("settings")->setLevel(Log::Log::eError);
                Log::Manager::get("json")->setLevel(Log::Log::eInfo);
                Log::Manager::get("lua")->setLevel(Log::Log::eInfo);
                Log::Manager::get("map")->setLevel(Log::Log::eInfo);
                Log::Manager::get("input")->setLevel(Log::Log::eInfo);
                Log::Manager::get("states.intro")->setLevel(Log::Log::eDebug);
                Log::Manager::get("states.main_menu")->setLevel(Log::Log::eDebug);
                Log::Manager::get("states.map")->setLevel(Log::Log::eInfo);
                Log::Manager::get("states.battle")->setLevel(Log::Log::eDebug);
                Log::Manager::get("assets.fonts")->setLevel(Log::Log::eDebug);
                Log::Manager::get("assets.songs")->setLevel(Log::Log::eDebug);
                Log::Manager::get("assets.sounds")->setLevel(Log::Log::eDebug);
                Log::Manager::get("assets.texures")->setLevel(Log::Log::eDebug);
            }

            LoggerManager::~LoggerManager() {
            }

            std::shared_ptr<Log::Logger> LoggerManager::GetLogger(const std::string& channelName) {
                return std::make_shared<Log::Logger>(channelName.c_str());
            }
        }
    }
}

