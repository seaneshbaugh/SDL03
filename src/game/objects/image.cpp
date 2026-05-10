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
                this->position.w = this->texture->GetSDLTexture().get()->w;

                this->position.h = this->texture->GetSDLTexture().get()->h;
            }
        }

        SDL_FRect Image::GetPosition() {
            return this->position;
        }

        float Image::GetX() {
            return this->position.x;
        }

        float Image::GetY() {
            return this->position.y;
        }

        float Image::GetWidth() {
            return this->position.w;
        }

        float Image::GetHeight() {
            return this->position.h;
        }

        void Image::SetPosition(float x, float y) {
            this->position = {x, y, this->position.w, this->position.h};
        }

        void Image::Render() {
            this->Render(nullptr);
        }

        void Image::Render(const float clipX, const float clipY, const float clipW, const float clipH) {
            SDL_FRect clip = {clipX, clipY, clipW, clipH};

            this->Render(&clip);
        }

        void Image::Render(const SDL_FRect* const clip) {
            if (this->texture == nullptr) {
                return;
            }

            SDL_FRect renderQuad = {this->position.x, this->position.y, this->position.w, this->position.h};

            if (clip != nullptr) {
                renderQuad.w = clip->w;
                renderQuad.h = clip->h;
            }

            Services::Locator::VideoService()->RenderTexture(this->texture, clip, &renderQuad);
        }

        void Image::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
            sol::table objects = (*luaState.get())["objects"].get_or_create<sol::table>(sol::new_table());

            objects.new_usertype<Image>("Image",
                                        sol::constructors<Image(), Image(const std::string&, const float, const float)>(),
                                        "getX", &Image::GetX,
                                        "getY", &Image::GetY,
                                        "getWidth", &Image::GetWidth,
                                        "getHeight", &Image::GetHeight,
                                        "setPosition", &Image::SetPosition,
                                        "render", sol::overload(static_cast<void (Image::*)()>(&Image::Render),
                                                                static_cast<void (Image::*)(const float, const float, const float, const float)>(&Image::Render))
                                        );
        }
    }
}
