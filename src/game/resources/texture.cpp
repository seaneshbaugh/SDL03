#include "texture.hpp"

namespace Game {
    namespace Resources {
        const std::string Texture::logChannel = "assets.texures";

        Texture::Texture() {
            this->logger = Services::Locator::LoggerService()->GetLogger(Texture::logChannel);
            this->sdlTexture = nullptr;
            this->filename = "";
        }

        Texture::Texture(const std::string& filename) : Texture() {
            this->Load(filename);
        }

        Texture::~Texture() {
            this->DestroySDLTexture();
        }

        void Texture::Load(const std::string& filename) {
            this->DestroySDLTexture();

            this->filename = filename;

            SDL_Surface* surface = IMG_Load(this->filename.c_str());

            if (!surface) {
                this->logger->error() << "Error creating surface from \"" << filename << "\":" << IMG_GetError();

                throw;
            }

            this->sdlTexture = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(Services::Locator::VideoService()->GetRenderer().get(), surface), SDL_DestroyTexture);

            SDL_FreeSurface(surface);

            if (!this->sdlTexture) {
                this->logger->error() << "Error creating texture from \"" << filename << "\":" << SDL_GetError();

                throw;
            }
        }

        std::shared_ptr<SDL_Texture> Texture::GetSDLTexture() {
            return this->sdlTexture;
        }

        void Texture::DestroySDLTexture() {
            if (this->sdlTexture == nullptr) {
                return;
            }

            this->logger->info() << "Destroying texture \"" << this->filename << "\".";

            // Now that this is a smart pointer this probably isn't needed.
            this->sdlTexture.reset();

            this->logger->info() << "Destroyed texture \"" << this->filename << "\".";
        }
    }
}
