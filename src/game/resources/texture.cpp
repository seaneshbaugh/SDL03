#include "texture.hpp"

namespace Game {
    namespace Resources {
        const std::string Texture::logChannel = "assets.texures";

        Texture::Texture() {
            this->logger = Services::Locator::LoggerService()->GetLogger(Texture::logChannel);
            this->texture = nullptr;
            this->filename = "";
        }

        Texture::Texture(const std::string& filename) : Texture() {
            this->Load(filename);
        }

        Texture::~Texture() {
            this->DestroyTexture();
        }

        void Texture::Load(const std::string& filename) {
            this->DestroyTexture();

            this->filename = filename;

            SDL_Surface* surface = IMG_Load(this->filename.c_str());

            if (!surface) {
                this->logger->error() << "Error creating surface from \"" << filename << "\":" << IMG_GetError();

                throw;
            }

            this->texture = SDL_CreateTextureFromSurface(Services::Locator::VideoService()->GetRenderer().get(), surface);

            SDL_FreeSurface(surface);

            if (!this->texture) {
                this->logger->error() << "Error creating texture from \"" << filename << "\":" << SDL_GetError();

                throw;
            }
        }

        bool Texture::DestroyTexture() {
            if (this->texture == nullptr) {
                return false;
            }

            this->logger->info() << "Destroying texture \"" << this->filename << "\".";

            SDL_DestroyTexture(this->texture);

            this->logger->info() << "Destroyed texture \"" << this->filename << "\".";

            this->texture = nullptr;

            return true;
        }
    }
}
