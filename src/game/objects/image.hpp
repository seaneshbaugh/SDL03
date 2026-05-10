#ifndef SDL03_Game_Objects_Image
#define SDL03_Game_Objects_Image

#include "base.hpp"
#include "../resources/texture.hpp"

namespace Game {
    namespace Objects {
        class Image : public Base {
        public:
            Image();
            Image(std::shared_ptr<Resources::Texture> texture, const int x, const int y);
            Image(const std::string& textureName, const int x, const int y);
            ~Image();
            void SetTexture(std::shared_ptr<Resources::Texture> texture);
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
            std::shared_ptr<Resources::Texture> texture;
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
