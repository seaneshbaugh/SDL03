#include "TextureManager.h"

namespace Services {
    namespace Implementations {
        TextureManager::TextureManager() {
        }

        TextureManager::~TextureManager() {
        }

        void TextureManager::AddTexture(std::shared_ptr<GameTexture> texture, const std::string textureName, GameState* addedBy) {
            this->textures[textureName] = std::make_pair(texture, addedBy);
        }

        std::shared_ptr<GameTexture> TextureManager::GetTexture(const std::string textureName) {
            return this->textures[textureName].first;
        }

        // This is really bad. I just haven't come up with a better way to indicate
        // which state added the texture. The intent is that when a state is popped
        // it calls ClearTextures with a weak_ptr to itself and then anything it added
        // is removed. I don't think I like the idea of other objects manually
        // triggering what is essentially garbage collection. Before now states have
        // managed their own texture lists. The problem with that is it ties creation
        // of GameImage objects to the state object in a way that is pretty messy.
        // Now that I'm trying to use smart pointers as much as possible it's actually
        // not even possible to do it that way. Before the pointers to the raw
        // GameTexture object could pass through Lua land without problem. Unfortunately
        // there's no real way to cast a void* to a shared_ptr (shared_ptr is supposed
        // to control object instantiation) so textures can't be passed into Lua land
        // and then subsequently taken out without issue. This means another way of
        // referencing textures is necessary. The addedBy business is currently
        // necessary because otherwise the textures would just hang out in the container
        // until the game closes.
        void TextureManager::ClearTextures(GameState* addedBy) {
            for (auto it = this->textures.begin(); it != this->textures.end();) {
                if (it->second.second == addedBy) {
                    this->textures.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }
}
