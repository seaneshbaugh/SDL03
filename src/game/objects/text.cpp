#include "text.hpp"

namespace Game {
    namespace Objects {
        Text::Text() {
            this->text = "";
            this->font = nullptr;
            this->color = {255, 255, 255};
            this->position = {0, 0, 0, 0};
            this->texture = nullptr;
        }

        Text::Text(const std::string& text, std::shared_ptr<Resources::Font> font, const int x, const int y, const SDL_Color& color) {
            this->text = text;
            this->font = font;
            this->color = color;
            this->position = {x, y, 0, 0};
            this->texture = nullptr;

            this->UpdateTexture();
        }

        Text::Text(const std::string& text, const std::string& fontName, const int fontSize, const int x, const int y, const Uint8 r, const Uint8 g, const Uint8 b) {
            this->text = text;
            this->font = Services::Locator::FontService()->GetFont(fontName, fontSize);
            this->color = {r, g, b};
            this->position = {x, y, 0, 0};
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

        void Text::SetText(const std::string& text) {
            this->text = text;

            this->UpdateTexture();
        }

        void Text::SetFont(std::shared_ptr<Resources::Font> font) {
            this->font = font;

            this->UpdateTexture();
        }

        void Text::SetFont(const std::string& fontName, const int fontSize) {
            this->SetFont(Services::Locator::FontService()->GetFont(fontName, fontSize));
        }

        SDL_Rect Text::GetPosition() {
            return this->position;
        }

        int Text::GetX() {
            return this->position.x;
        }

        int Text::GetY() {
            return this->position.y;
        }

        int Text::GetWidth() {
            return this->position.w;
        }

        int Text::GetHeight() {
            return this->position.h;
        }

        void Text::SetPosition(int x, int y) {
            int w = 0;
            int h = 0;

            if (this->texture) {
                SDL_QueryTexture(this->texture, nullptr, nullptr, &w, &h);
            }

            this->position = {x, y, w, h};
        }

        void Text::SetColor(const SDL_Color& color) {
            this->color = color;

            this->UpdateTexture();
        }

        void Text::SetColor(const Uint8 r, const Uint8 g, const Uint8 b) {
            this->SetColor({r, g, b});
        }

        void Text::Render() {
            if (this->texture == nullptr) {
                return;
            }

            Services::Locator::VideoService()->Render(this->texture, nullptr, &this->position);
        }

        void Text::UpdateTexture() {
            if (this->texture) {
                SDL_DestroyTexture(this->texture);

                this->texture = nullptr;
            }

            SDL_Surface* textSurface = TTF_RenderText_Blended(this->font->GetTTFFont().get(), this->text.c_str(), this->color);

            this->texture = SDL_CreateTextureFromSurface(Services::Locator::VideoService()->GetRenderer().get(), textSurface);

            SDL_FreeSurface(textSurface);

            this->SetPosition(this->position.x, this->position.y);
        }

        void Text::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
            sol::table objects = (*luaState.get())["objects"].get_or_create<sol::table>(sol::new_table());

            objects.new_usertype<Text>("Text",
                                       sol::constructors<Text(), Text(const std::string&, const std::string&, const int, const int, const int, const Uint8, const Uint8, const Uint8)>(),
                                       "getText", &Text::GetText,
                                       "setText", &Text::SetText,
                                       "getX", &Text::GetX,
                                       "getY", &Text::GetY,
                                       "getWidth", &Text::GetWidth,
                                       "getHeight", &Text::GetHeight,
                                       "setPosition", &Text::SetPosition,
                                       "setColor", static_cast<void (Text::*)(const Uint8, const Uint8, const Uint8)>(&Text::SetColor),
                                       "render", &Text::Render
                                       );
        }

        // TODO: Remove once all states are using sol.
        void Text::LuaInterface::BindOld(std::shared_ptr<LuaIntf::LuaContext> luaContext) {
            LuaIntf::LuaBinding(luaContext->state())
            .beginModule("objects")
                .beginClass<Text>("Text")
                    .addConstructor(LUA_ARGS())
                    .addConstructor(LUA_ARGS(std::string, std::string, const int, const int, const int, const Uint8, const Uint8, const Uint8))
                    .addFunction("getText", &Text::GetText)
                    .addFunction("setText", &Text::SetText)
                    .addFunction("getX", &Text::GetX)
                    .addFunction("getY", &Text::GetY)
                    .addFunction("getWidth", &Text::GetWidth)
                    .addFunction("getHeight", &Text::GetHeight)
                    .addFunction("setPosition", &Text::SetPosition)
                    .addFunction("setColor", static_cast<void(Game::Objects::Text::*)(const Uint8, const Uint8, const Uint8)>(&Text::SetColor))
                    .addFunction("render", &Text::Render)
                .endClass()
            .endModule();
        }
    }
}
