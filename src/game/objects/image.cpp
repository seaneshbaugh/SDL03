#include "image.hpp"

namespace Game {
    namespace Objects {
        Image::Image() {
            this->texture = nullptr;
            this->position = {0, 0, 0, 0};
        }

        Image::Image(std::shared_ptr<Resources::Texture> texture, const int x, const int y) : Image() {
            this->SetTexture(texture);
            this->SetPosition(x, y);
        }

        Image::Image(const std::string& textureName, const int x, const int y) : Image() {
            this->SetTexture(Services::Locator::TextureService()->GetTexture(textureName));
            this->SetPosition(x, y);
        }

        Image::~Image() {
        }

        void Image::SetTexture(std::shared_ptr<Resources::Texture> texture) {
            this->texture = texture;

            if (this->texture) {
                SDL_QueryTexture(this->texture->GetSDLTexture().get(), nullptr, nullptr, &this->position.w, &this->position.h);
            }
        }

        SDL_Rect Image::GetPosition() {
            return this->position;
        }

        int Image::GetX() {
            return this->position.x;
        }

        int Image::GetY() {
            return this->position.y;
        }

        int Image::GetWidth() {
            return this->position.w;
        }

        int Image::GetHeight() {
            return this->position.h;
        }

        void Image::SetPosition(int x, int y) {
            this->position = {x, y, this->position.w, this->position.h};
        }

        void Image::Render(const SDL_Rect* const clip) {
            if (this->texture == nullptr) {
                
                return;
            }

            SDL_Rect renderQuad = {this->position.x, this->position.y, this->position.w, this->position.h};

            if (clip != nullptr) {
                renderQuad.w = clip->w;
                renderQuad.h = clip->h;
            }

            Services::Locator::VideoService()->Render(this->texture, clip, &renderQuad);
        }

        void Image::Render(const int clipX, const int clipY, const int clipW, const int clipH) {
            if (this->texture == nullptr) {
                return;
            }

            SDL_Rect clip = {clipX, clipY, clipW, clipH};

            SDL_Rect renderQuad = {this->position.x, this->position.y, clipW, clipH};

            Services::Locator::VideoService()->Render(this->texture, &clip, &renderQuad);
        }

        void Image::LuaInterface::Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext) {
            LuaIntf::LuaBinding(luaContext->state())
            .beginModule("objects")
                .beginClass<Image>("Image")
                    .addConstructor(LUA_ARGS())
                    .addConstructor(LUA_ARGS(std::string, const int, const int))
                    .addFunction("getX", &Image::GetX)
                    .addFunction("getY", &Image::GetY)
                    .addFunction("getWidth", &Image::GetWidth)
                    .addFunction("getHeight", &Image::GetHeight)
                    .addFunction("setPosition", &Image::SetPosition)
                    //.addFunction("render", static_cast<void(Game::Objects::Image::*)(LuaIntf::_def<const SDL_Rect* const, nullptr>)>(&Image::Render))
                    //.addFunction("render", static_cast<void(Game::Objects::Image::*)()>(&Image::Render))
                    .addFunction("render", [](Image* self) { self->Render(); })
                    //.addFunction("renderWithClip", static_cast<void(Game::Objects::Image::*)(const int, const int, const int, const int)>(&Image::Render))
                    .addFunction("renderWithClip", [](Image* self, int x, int y, int w, int h) {
                        SDL_Rect clip = {x, y, w, h};

                        self->Render(&clip);
                    })
                .endClass()
            .endModule();
        }
    }
}
