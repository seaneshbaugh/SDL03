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

            bool VideoManager::Render(SDL_Texture* texture, const SDL_Rect* const srcrect, const SDL_Rect* const dstrect) {
                // TODO: Remove this temporary shim to convert SDL_Rect to SDL_FRect.
                SDL_FRect srcf;
                SDL_FRect dstf;

                SDL_FRect* srcp = nullptr;
                SDL_FRect* dstp = nullptr;

                if (srcrect) {
                    srcf = SDL_FRect{
                        static_cast<float>(srcrect->x),
                        static_cast<float>(srcrect->y),
                        static_cast<float>(srcrect->w),
                        static_cast<float>(srcrect->h)};

                    srcp = &srcf;
                }

                if (dstrect) {
                    dstf = SDL_FRect{
                        static_cast<float>(dstrect->x),
                        static_cast<float>(dstrect->y),
                        static_cast<float>(dstrect->w),
                        static_cast<float>(dstrect->h)};

                    dstp = &dstf;
                }

                if (SDL_RenderTexture(this->renderer.get(), texture, srcp, dstp) == false) {
                    this->logger->error() << "Failed to render texture: " << SDL_GetError();

                    return false;
                }

                return true;
            }

            bool VideoManager::Render(std::shared_ptr<Resources::Texture> texture, const SDL_Rect* const srcrect, const SDL_Rect* const dstrect) {
                return this->Render(texture->GetSDLTexture().get(), srcrect, dstrect);
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
