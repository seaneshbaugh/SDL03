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
            ~Image();
            void SetTexture(std::shared_ptr<Resources::Texture> texture);
            SDL_Rect GetPosition();
            void SetPosition(int x, int y);
            void Render(const SDL_Rect* const clip = nullptr);

        private:
            std::shared_ptr<Resources::Texture> texture;
            int x;
            int y;
            int width;
            int height;
            SDL_Rect textureLocation;

            void UpdateTexture();

        public:
            class LuaInterface {
            public:
                static void Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext);
            };
        };
    }
}


#endif
