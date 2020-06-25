#ifndef SDL03_Game_Services_Interfaces_VideoService
#define SDL03_Game_Services_Interfaces_VideoService

#include <memory>

#include <SDL.h>

namespace Game {
    namespace Resources {
        class Texture;
    }

    namespace Services {
        namespace Interfaces {
            class VideoService {
            public:
                ~VideoService() {}
                virtual void ClearScreen() = 0;
                virtual bool Render(SDL_Texture* texture, const SDL_Rect* const srcrect, const SDL_Rect* const dstrect) = 0;
                virtual bool Render(std::shared_ptr<Resources::Texture> texture, const SDL_Rect* const srcrect, const SDL_Rect* const dstrect) = 0;
                virtual void UpdateScreen() = 0;
                virtual std::shared_ptr<SDL_Renderer> GetRenderer() = 0;
            };
        }
    }
}

#endif
