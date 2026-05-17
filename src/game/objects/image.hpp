#ifndef SDL03_Game_Objects_Image
#define SDL03_Game_Objects_Image

#include "base.hpp"
#include "../assets/texture.hpp"

namespace Game {
    // TODO: Move this to Graphics namespace. Maybe even get rid of it entirely. Or
    // have it just be a thin wrapper around a single static from a Spritesheet? For
    // now I'm leaving it here.
    namespace Objects {
        class Image : public Base {
        public:
            Image();
            Image(std::shared_ptr<Assets::Texture> texture, const float x, const float y);
            Image(const std::string& textureName, const float x, const float y);
            ~Image();
            void SetTexture(std::shared_ptr<Assets::Texture> texture);
            SDL_FRect GetPosition();
            void SetPosition(float x, float y);
            float GetX();
            float GetY();
            float GetWidth();
            float GetHeight();
            void Render();
            void Render(const SDL_FRect* const clip);
            void Render(const float clipX, const float clipY, const float clipW, const float clipH);

        private:
            std::shared_ptr<Assets::Texture> texture;
            SDL_FRect position;
            void UpdateTexture();

        public:
            class LuaInterface {
            public:
                static void Bind(std::shared_ptr<sol::state> luaState);
            };
        };
    }
}

#endif
