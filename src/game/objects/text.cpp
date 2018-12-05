#include "text.hpp"

namespace Game {
    namespace Objects {
        Text::Text() {
            this->text = "";
            this->font = nullptr;
            this->x = 0;
            this->y = 0;
            this->color = {255, 255, 255};
            this->textLocation = {0, 0, 0, 0};
            this->texture = nullptr;
        }

        Text::Text(std::string text, std::shared_ptr<Resources::Font> font, const int x, const int y, SDL_Color color, SDL_Rect location) {
            this->text = text;
            this->font = font;
            this->x = x;
            this->y = y;
            this->color = color;
            this->textLocation = location;
            this->texture = nullptr;

            this->UpdateTexture();
        }

        Text::~Text() {
            if (this->texture) {
                SDL_DestroyTexture(this->texture);
            }
        }

        std::string Text::GetText() {
            return this->text;
        }

        void Text::SetText(std::string text) {
            this->text = text;

            this->UpdateTexture();
        }

        void Text::SetFont(std::shared_ptr<Resources::Font> font) {
            this->font = font;

            this->UpdateTexture();
        }

        SDL_Rect Text::GetPosition() {
            return this->textLocation;
        }

        int Text::GetX() {
            return this->textLocation.x;
        }

        int Text::GetY() {
            return this->textLocation.y;
        }

        int Text::GetWidth() {
            return this->textLocation.w;
        }

        int Text::GetHeight() {
            return this->textLocation.h;
        }

        void Text::SetPosition(int x, int y) {
            this->x = x;
            this->y = y;

            int w = 0;
            int h = 0;

            if (this->texture) {
                SDL_QueryTexture(this->texture, nullptr, nullptr, &w, &h);
            }

            this->textLocation = {x, y, w, h};
        }

        void Text::SetColor(SDL_Color color) {
            this->color = color;

            this->UpdateTexture();
        }

        void Text::Render() {
            if (this->texture == nullptr) {
                return;
            }

            Services::Locator::VideoService()->Render(this->texture, nullptr, &this->textLocation);
        }

        void Text::UpdateTexture() {
            if (this->texture) {
                SDL_DestroyTexture(this->texture);

                this->texture = nullptr;
            }

            SDL_Surface* textSurface = TTF_RenderText_Blended(this->font->font, this->text.c_str(), this->color);

            this->texture = SDL_CreateTextureFromSurface(Services::Locator::VideoService()->GetRenderer().get(), textSurface);

            SDL_FreeSurface(textSurface);

            this->SetPosition(this->x, this->y);
        }

        void Text::LuaInterface::Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext) {
            LuaIntf::LuaBinding(luaContext->state())
            .beginModule("objects")
            .beginClass<Text>("Text")
            .addConstructor(LUA_ARGS())
            .addConstructor(LUA_ARGS(std::string, std::shared_ptr<Resources::Font>, const int, const int, SDL_Color, SDL_Rect))
            .addFunction("getText", &Text::GetText)
            .addFunction("setText", &Text::SetText)
            .addFunction("getX", &Text::GetX)
            .addFunction("getY", &Text::GetY)
            .addFunction("getWidth", &Text::GetWidth)
            .addFunction("getHeight", &Text::GetHeight)
            .addFunction("setPosition", &Text::SetPosition)
            .addFunction("setColor", &Text::SetColor)
            .addFunction("render", &Text::Render)
            .endClass()
            .endModule();
        }
    }
}
