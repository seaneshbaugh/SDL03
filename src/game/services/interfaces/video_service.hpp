#ifndef SDL03_Game_Services_Interfaces_VideoService
#define SDL03_Game_Services_Interfaces_VideoService

#include <memory>

#include <SDL3/SDL.h>

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
                // TODO: Make the version of this function that takes a raw pointer private.
                // Ideally we should only publicly expose the version that takes a shared pointer.
                // The version that takes a raw pointer can be used internally by the version that takes a shared pointer.
                virtual bool RenderTexture(SDL_Texture* texture, const SDL_FRect* const srcrect, const SDL_FRect* const dstrect) = 0;
                virtual bool RenderTexture(std::shared_ptr<Resources::Texture> texture, const SDL_FRect* const srcrect, const SDL_FRect* const dstrect) = 0;
                virtual void UpdateScreen() = 0;
                virtual std::shared_ptr<SDL_Renderer> GetRenderer() = 0;
            };
        }
    }
}

#endif
