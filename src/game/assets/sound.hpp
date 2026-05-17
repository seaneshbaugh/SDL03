#ifndef SDL03_Game_Resources_Sound
#define SDL03_Game_Resources_Sound

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "base.hpp"

namespace Game {
    namespace Assets {
        class Sound : public Base {
        public:
            MIX_Audio* audio;

            Sound();
            Sound(const std::string& filepath);
            ~Sound();
            void Load(const std::string& filepath);

        private:
            static const std::string logChannel;

            std::string filename;

            bool DestroyAudio();
        };
    }
}

#endif
