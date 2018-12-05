#ifndef SDL03_Game_Services_Interfaces_TextureService
#define SDL03_Game_Services_Interfaces_TextureService

#include <memory>
#include <string>

namespace Game {
    namespace States {
        class Base;
    }

    namespace Resources {
        class Texture;
    }

    namespace Services {
        namespace Interfaces {
            class TextureService {
            public:
                virtual ~TextureService() {}
                virtual void AddTexture(std::shared_ptr<Resources::Texture> texture, const std::string& textureName, std::shared_ptr<States::Base> addedBy) = 0;
                virtual std::shared_ptr<Resources::Texture> GetTexture(const std::string& textureName) = 0;
                virtual void ClearTextures(std::shared_ptr<States::Base> addedBy) = 0;
            };
        }
    }
}

#endif
