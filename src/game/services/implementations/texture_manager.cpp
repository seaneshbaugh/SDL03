#include "texture_manager.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            const std::string TextureManager::logChannel = "assets.textures";

            TextureManager::TextureManager() {
                if (Locator::LoggerService() == nullptr) {
                    throw std::runtime_error("LoggerService must be started before TextureService.");
                }

                this->logger = Locator::LoggerService()->GetLogger(TextureManager::logChannel);

                if ((IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG) != IMG_INIT_PNG) {
                    this->logger->critic() << "Failed to initialize SDL_image: " << IMG_GetError();

                    throw;
                }

                this->logger->info() << "SDL_image initialized.";
            }

            TextureManager::~TextureManager() {
                IMG_Quit();
            }

            std::shared_ptr<Resources::Texture> TextureManager::AddTexture(const std::string& textureName, const std::string& filename) {
                auto it = this->textures.find(textureName);

                if (it != this->textures.end()) {
                    it->second.second++;

                    return it->second.first;
                }

                auto texture = std::make_shared<Resources::Texture>(filename);

                this->textures[textureName] = std::make_pair(texture, 1);

                return texture;
            }

            std::shared_ptr<Resources::Texture> TextureManager::GetTexture(const std::string& textureName) {
                auto it = this->textures.find(textureName);

                if (it != this->textures.end()) {
                    it->second.second++;

                    return it->second.first;
                }

                return nullptr;
            }

            void TextureManager::ReleaseTexture(const std::string& textureName) {
                auto it = this->textures.find(textureName);

                if (it != this->textures.end()) {
                    it->second.second--;

                    if (it->second.second == 0) {
                        this->textures.erase(it);
                    }
                }
            }
        }
    }
}
