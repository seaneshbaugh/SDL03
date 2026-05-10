#include "video_manager.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            const std::string VideoManager::logChannel = "video";

            VideoManager::VideoManager(const int screenWidth, const int screenHeight, const std::string& windowTitle) {
                if (Locator::LoggerService() == nullptr) {
                    throw std::runtime_error("LoggerService must be started before VideoService.");
                }

                this->logger = Locator::LoggerService()->GetLogger(VideoManager::logChannel);

                if (SDL_InitSubSystem(SDL_INIT_VIDEO) == false) {
                    this->logger->critic() << "Failed to initialize SDL video: " << SDL_GetError();

                    throw;
                }

                this->logger->info() << "SDL video initialized.";

                this->screenWidth = screenWidth;

                this->screenHeight = screenHeight;

                this->windowTitle = windowTitle;

                this->window = std::shared_ptr<SDL_Window>(SDL_CreateWindow(this->windowTitle.c_str(), this->screenWidth, this->screenHeight, SDL_WINDOW_OPENGL), SDL_DestroyWindow);

                if (this->window == nullptr) {
                    this->logger->critic() << "Failed to create window: " << SDL_GetError();

                    throw;
                }

                this->logger->info() << "Window created.";

                this->renderer = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(this->window.get(), NULL), SDL_DestroyRenderer);

                if (this->renderer == nullptr) {
                    this->logger->critic() << "Failed to create renderer: " << SDL_GetError();

                    throw;
                }

                this->logger->info() << "Renderer created.";
            }

            VideoManager::~VideoManager() {
                SDL_QuitSubSystem(SDL_INIT_VIDEO);
            }

            void VideoManager::ClearScreen() {
                SDL_SetRenderDrawColor(this->renderer.get(), 0, 0, 0, 255);

                SDL_RenderClear(this->renderer.get());
            }

            bool VideoManager::RenderTexture(SDL_Texture* texture, const SDL_FRect* const srcrect, const SDL_FRect* const dstrect) {
                if (SDL_RenderTexture(this->renderer.get(), texture, srcrect, dstrect) == false) {
                    this->logger->error() << "Failed to render texture: " << SDL_GetError();

                    return false;
                }

                return true;
            }

            bool VideoManager::RenderTexture(std::shared_ptr<Resources::Texture> texture, const SDL_FRect* const srcrect, const SDL_FRect* const dstrect) {
                return this->RenderTexture(texture->GetSDLTexture().get(), srcrect, dstrect);
            }

            void VideoManager::UpdateScreen() {
                SDL_RenderPresent(this->renderer.get());
            }

            std::shared_ptr<SDL_Renderer> VideoManager::GetRenderer() {
                return this->renderer;
            }
        }
    }
}
