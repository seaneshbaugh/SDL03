#ifndef SDL03_Game_Services_Interfaces_AudioService
#define SDL03_Game_Services_Interfaces_AudioService

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

namespace Game {
    namespace Services {
        namespace Interfaces {
            class AudioService {
            public:
                virtual ~AudioService() {}
                virtual MIX_Mixer* GetMixer() = 0;
            };
        }
    }
}

#endif
