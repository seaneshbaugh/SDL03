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
            class TextureManager : public Interfaces::TextureService {
            public:
                TextureManager();
                ~TextureManager();
                void AddTexture(std::shared_ptr<Resources::Texture> texture, const std::string& textureName, std::shared_ptr<States::Base> addedBy);
                std::shared_ptr<Resources::Texture> GetTexture(const std::string& textureName);
                void ClearTextures(std::shared_ptr<States::Base> addedBy);
            private:
                static const std::string logChannel;

                std::shared_ptr<Log::Logger> logger;
                std::map<std::string, std::pair<std::shared_ptr<Resources::Texture>, std::shared_ptr<States::Base>>> textures;
            };
        }
    }
}

#endif
