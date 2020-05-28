#ifndef SDL03_Game_Resources_Texture
#define SDL03_Game_Resources_Texture

#include "SDL.h"
#include "SDL_image.h"

#include "base.hpp"

namespace Game {
    class Engine;

    namespace Resources {
        class Texture : public Base {
        public:
            Texture();
            Texture(const std::string& filename);
            ~Texture();
            void Load(const std::string& filename);
            std::shared_ptr<SDL_Texture> GetSDLTexture();

        private:
            static const std::string logChannel;

            std::shared_ptr<Log::Logger> logger;
            std::shared_ptr<SDL_Texture> sdlTexture;

            void DestroySDLTexture();
        };
    }
}

#endif
