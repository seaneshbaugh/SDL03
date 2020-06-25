#ifndef SDL03_Game_Engine
#define SDL03_Game_Engine

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <sys/param.h>
#elif _WIN32
#error Windows is not yet supported.
#elif __linux
#error Linux is not yet supported.
#else
#error Unknown and unsupported platform.
#endif

#include <memory>
#include <stack>
#include <string>

#include <SDL.h>

#include "../../lib/LoggerCpp/include/LoggerCpp/LoggerCpp.h"

#include "../game.hpp"
#include "services/locator.hpp"
#include "services/implementations/audio_manager.hpp"
#include "services/implementations/clock.hpp"
#include "services/implementations/font_manager.hpp"
#include "services/implementations/input_manager.hpp"
#include "services/implementations/logger_manager.hpp"
#include "services/implementations/settings_manager.hpp"
#include "services/implementations/texture_manager.hpp"
#include "services/implementations/video_manager.hpp"
#include "services/implementations/world_manager.hpp"
#include "services/implementations/save_manager.hpp"
#include "states/base.hpp"
#include "states/intro.hpp"

namespace Game {
    class Engine {
    public:
        Engine();
        ~Engine();
        int Start();

    private:
        static const std::string logChannel;

        std::shared_ptr<Log::Logger> logger;
        std::stack<std::shared_ptr<States::Base>> states;

        void StartSystem();
        void StartLoggerService();
        void StartTimeService();
        void StartSettingsService();
        void StartSDL();
        void StartVideoService();
        void StartTextureService();
        void StartFontService();
        void StartAudioService();
        void StartInputService();
        void StartWorldService();
        void StartSaveService();
        void MainLoop();
        void Render();
        void DestroyStates();
        void StopSaveService();
        void StopWorldService();
        void StopInputService();
        void StopAudioService();
        void StopFontService();
        void StopTextureService();
        void StopVideoService();
        void StopSDL();
        void StopSettingsService();
        void StopTimeService();
        void StopLoggerService();
        void StopSystem();
    };
}

#endif
