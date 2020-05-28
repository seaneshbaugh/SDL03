#ifndef SDL03_Game_Objects_Text
#define SDL03_Game_Objects_Text

#include <string>
#include <iostream>

#include "../services/locator.hpp"
#include "../resources/font.hpp"
#include "base.hpp"

namespace Game {
    namespace Objects {
        class Text : public Base {
        public:
            Text();
            Text(const std::string& text, std::shared_ptr<Resources::Font> font, const int x, const int y, const SDL_Color& color);
            Text(const std::string& text, const std::string& fontName, const int fontSize, const int x, const int y, const Uint8 r, const Uint8 g, const Uint8 b);
            ~Text();
            std::string GetText();
            void SetText(const std::string& text);
            void SetFont(std::shared_ptr<Resources::Font> font);
            void SetFont(const std::string& fontName, const int fontSize);
            SDL_Rect GetPosition();
            int GetX();
            int GetY();
            int GetWidth();
            int GetHeight();
            void SetPosition(const int x, const int y);
            SDL_Color GetColor();
            void SetColor(const SDL_Color& color);
            void SetColor(const Uint8 r, const Uint8 g, const Uint8 b);
            void Render();

        private:
            std::string text;
            std::shared_ptr<Resources::Font> font;
            SDL_Rect position;
            SDL_Color color;
            SDL_Texture* texture;

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
