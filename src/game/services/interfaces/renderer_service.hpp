#ifndef SDL03_Game_Services_Interfaces_RendererService
#define SDL03_Game_Services_Interfaces_RendererService

#include <memory>

#include <SDL.h>

#include "../../resources/texture.hpp"

namespace Game {
    namespace Services {
        namespace Interfaces {
            class RendererService {
            public:
                virtual ~RendererService() {}
                virtual bool Render(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect) = 0;
                virtual bool Render(std::shared_ptr<Resources::Texture> texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect) = 0;
                virtual std::shared_ptr<SDL_Renderer> GetRenderer() = 0;
            };
        }
    }
}

#endif
