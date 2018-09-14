#ifndef __SDL03__TextureService__
#define __SDL03__TextureService__

#include <utility>

#include "SDL.h"

#include "GameTexture.h"

class GameState;

namespace Services {
    namespace Interfaces {
        class TextureService {
        public:
            virtual ~TextureService() {}
            virtual void AddTexture(std::shared_ptr<GameTexture> texture, const std::string textureName, GameState* addedBy) = 0;
            virtual std::shared_ptr<GameTexture> GetTexture(const std::string textureName) = 0;
            virtual void ClearTextures(GameState* addedBy) = 0;
        };
    }
}

#endif
