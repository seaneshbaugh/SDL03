#ifndef SDL03_Game_Resources_Song
#define SDL03_Game_Resources_Song

#include <SDL.h>
#include <SDL_mixer.h>

#include "base.hpp"
#include "../services/locator.hpp"

namespace Game {
    namespace Resources {
        class Song : public Base {
        public:
            Mix_Music* music;

            Song();
            Song(const std::string& filename);
            ~Song();
            void Load(const std::string& filename);

        private:
            std::string filename;

            bool DestroyMusic();
        };
    }
}

#endif
