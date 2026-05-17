#ifndef SDL03_Game_Services_Interfaces_TextureService
#define SDL03_Game_Services_Interfaces_TextureService

#include <memory>
#include <string>

namespace Game {
    namespace Assets {
        class Texture;
    }

    namespace Services {
        namespace Interfaces {
            class TextureService {
            public:
                virtual ~TextureService() {}
                virtual std::shared_ptr<Assets::Texture> AddTexture(const std::string& textureName, const std::string& filename) = 0;
                virtual std::shared_ptr<Assets::Texture> GetTexture(const std::string& textureName) = 0;
                virtual void ReleaseTexture(const std::string& textureName) = 0;
            };
        }
    }
}

#endif
