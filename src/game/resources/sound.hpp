#ifndef SDL03_Game_Resources_Sound
#define SDL03_Game_Resources_Sound

#include <SDL.h>
#include <SDL_mixer.h>

#include "base.hpp"

namespace Game {
    namespace Resources {
        class Sound : public Base {
        public:
            Mix_Chunk* chunk;

            Sound();
            Sound(const std::string& filepath);
            ~Sound();
            void Load(const std::string& filepath);

        private:
            static const std::string logChannel;

            std::string filename;

            bool DestroyChunk();
        };
    }
}

#endif
