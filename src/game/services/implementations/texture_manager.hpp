#ifndef SDL03_Game_Services_Implementations_TextureManager
#define SDL03_Game_Services_Implementations_TextureManager

#include <utility>

#include "SDL.h"
#include "SDL_image.h"

#include "../../states/base.hpp"
#include "../locator.hpp"
#include "../interfaces/texture_service.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            class TextureManager : public Interfaces::TextureService /*, public ResourceManager<Resources::Texture>*/ {
            public:
                TextureManager();
                ~TextureManager();
                std::shared_ptr<Resources::Texture> AddTexture(const std::string& textureName, const std::string& filename);
                std::shared_ptr<Resources::Texture> GetTexture(const std::string& textureName);
                void ReleaseTexture(const std::string& textureName);

            private:
                static const std::string logChannel;

                std::shared_ptr<Log::Logger> logger;
                std::map<std::string, std::pair<std::shared_ptr<Resources::Texture>, unsigned int>> textures;
            };
        }
    }
}

#endif
