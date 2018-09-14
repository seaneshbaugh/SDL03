#include "RendererManager.h"

namespace Services {
    namespace Implementations {
        RendererManager::RendererManager(const int screenWidth, const int screenHeight, const std::string windowTitle) {
            this->screenWidth = screenWidth;

            this->screenHeight = screenHeight;

            this->windowTitle = windowTitle;

            this->window = std::shared_ptr<SDL_Window>(SDL_CreateWindow(this->windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->screenWidth, this->screenHeight, SDL_WINDOW_OPENGL), SDL_DestroyWindow);

            if (this->window == nullptr) {
                throw;
            }

            this->renderer = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(this->window.get(), -1, SDL_RENDERER_ACCELERATED), SDL_DestroyRenderer);

            if (this->renderer == nullptr) {
                throw;
            }

//            if (SDL_CreateWindowAndRenderer(this->screenWidth, this->screenHeight, SDL_WINDOW_OPENGL, this->window, this->renderer)) {
//                    // TODO: Figure out how to handle logging within services.
//                    // If this ends up going the route of having logging be a service,
//                    // which I still think is a good idea, then there needs to be some
//                    // sort of way to ensure that services are available before being
//                    // called.
//                    // this->logger->critic() << "Failed to create window and renderer. " << SDL_GetError();
//
//                throw;
//            }

            // this->logger->info() << "Window and render created.";

//            SDL_SetWindowTitle(this->window.get(), this->windowTitle.c_str());
        }

        RendererManager::~RendererManager() {
        }

        bool RendererManager::Render(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect) {
            SDL_RenderCopy(this->renderer.get(), texture, srcrect, dstrect);

            return true;
        }

        bool RendererManager::Render(std::shared_ptr<GameTexture> texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect) {
            SDL_RenderCopy(this->renderer.get(), texture->texture, srcrect, dstrect);

            return true;
        }

        std::shared_ptr<SDL_Renderer> RendererManager::GetRenderer() {
            return this->renderer;
        }
    }
}
