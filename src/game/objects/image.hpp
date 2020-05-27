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
            SDL_Rect GetPosition();
            void SetPosition(int x, int y);
            int GetX();
            int GetY();
            int GetWidth();
            int GetHeight();
            void Render();
            void Render(const SDL_Rect* const clip);
            void Render(const int clipX, const int clipY, const int clipW, const int clipH);

        private:
            std::shared_ptr<Resources::Texture> texture;
            SDL_Rect position;

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
