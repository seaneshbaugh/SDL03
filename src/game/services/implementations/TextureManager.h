#ifndef __SDL03__TextureManager__
#define __SDL03__TextureManager__

#include <memory>

#include "../interfaces/TextureService.h"

namespace Services {
    namespace Implementations {
        class TextureManager : public Interfaces::TextureService {
        public:
            TextureManager();
            ~TextureManager();
            void AddTexture(std::shared_ptr<GameTexture> texture, const std::string textureName, GameState* addedBy);
            std::shared_ptr<GameTexture> GetTexture(const std::string textureName);
            void ClearTextures(GameState* addedBy);
        private:
            std::map<std::string, std::pair<std::shared_ptr<GameTexture>, GameState*>> textures;
        };
    }
}

#endif
