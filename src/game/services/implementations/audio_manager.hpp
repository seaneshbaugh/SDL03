#ifndef SDL03_Game_Services_Implementations_AudioManager
#define SDL03_Game_Services_Implementations_AudioManager

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "../locator.hpp"
#include "../interfaces/audio_service.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            class AudioManager : public Interfaces::AudioService {
            public:
                AudioManager();
                ~AudioManager();
                MIX_Mixer* GetMixer();

                // TODO: Add MIX_Track* for Songs and Sounds.
                // See https://lazyfoo.net/tutorials/SDL3/15-sound-effects-and-music/index.php for more info.
                // TODO: Allow Songs to be loaded via name (figure out a way to map name to file name via convention).
                // TODO: Allow Sounds to be loaded via name.
                // When a Song is loaded it should begin playing immediately.
                // Only one song should be loaded at a time
                // Sounds should only be played when triggered by something.
                // Many sounds can be loaded at once.

            private:
                static const std::string logChannel;

                std::shared_ptr<Log::Logger> logger;
                // TODO: Wrap this in a std::shared_ptr.
                MIX_Mixer* mixer;
            };
        }
    }
}

#endif
