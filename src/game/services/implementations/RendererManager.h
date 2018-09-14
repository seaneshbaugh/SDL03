#ifndef __SDL03__RendererManager__
#define __SDL03__RendererManager__

#include <memory>

#include "../interfaces/RendererService.h"

namespace Services {
    namespace Implementations {
        class RendererManager : public Interfaces::RendererService {
        public:
            RendererManager(const int screenWidth, const int screenHeight, const std::string windowTitle);
            ~RendererManager();
            bool Render(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect);
            bool Render(std::shared_ptr<GameTexture> texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect);
            std::shared_ptr<SDL_Renderer> GetRenderer();
        private:
            std::shared_ptr<SDL_Window> window;
            std::shared_ptr<SDL_Renderer> renderer;
            int screenWidth;
            int screenHeight;
            std::string windowTitle;
        };
    }
}

#endif
