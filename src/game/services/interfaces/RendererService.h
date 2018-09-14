#ifndef __SDL03__RendererService__
#define __SDL03__RendererService__

#include <memory>

#include "SDL.h"

#include "GameTexture.h"

namespace Services {
    namespace Interfaces {
        class RendererService {
        public:
            virtual ~RendererService() {}
            virtual bool Render(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect) = 0;
            virtual bool Render(std::shared_ptr<GameTexture> texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect) = 0;
            virtual std::shared_ptr<SDL_Renderer> GetRenderer() = 0;
        };
    }
}

#endif
