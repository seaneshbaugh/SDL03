#ifndef SDL03_Game_Resources_Song
#define SDL03_Game_Resources_Song

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "base.hpp"
#include "../services/locator.hpp"

namespace Game {
    namespace Assets {
        class Song : public Base {
        public:
            MIX_Audio* audio;

            Song();
            Song(const std::string& filename);
            ~Song();
            void Load(const std::string& filename);

        private:
            std::string filename;

            bool DestroyAudio();
        };
    }
}

#endif
