#ifndef SDL03_Game_Services_Implementations_AudioManager
#define SDL03_Game_Services_Implementations_AudioManager

#include "SDL.h"
#include "SDL_mixer.h"

#include "../locator.hpp"
#include "../interfaces/audio_service.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            class AudioManager : public Interfaces::AudioService {
            public:
                AudioManager();
                ~AudioManager();

            private:
                static const std::string logChannel;

                std::shared_ptr<Log::Logger> logger;
            };
        }
    }
}

#endif
