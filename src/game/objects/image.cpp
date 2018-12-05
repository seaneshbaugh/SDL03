#include "image.hpp"

namespace Game {
    namespace Objects {
        Image::Image() {
            this->texture = nullptr;

            this->x = 0;

            this->y = 0;

            this->textureLocation = {0, 0, 0, 0};
        }

        Image::Image(std::shared_ptr<Resources::Texture> texture, int x, int y) : Image() {
            this->SetTexture(texture);

            this->SetPosition(x, y);
        }

        Image::~Image() {
        }

        void Image::SetTexture(std::shared_ptr<Resources::Texture> texture) {
            this->texture = texture;

            if (this->texture) {
                SDL_QueryTexture(this->texture->texture, nullptr, nullptr, &this->width, &this->height);
            }
        }

        SDL_Rect Image::GetPosition() {
            return this->textureLocation;
        }

        void Image::SetPosition(int x, int y) {
            this->x = x;

            this->y = y;

            this->textureLocation = {this->x, this->y, this->width, this->height};
        }

        void Image::Render(const SDL_Rect* const clip) {
            if (this->texture == nullptr) {
                return;
            }

            SDL_Rect renderQuad = {this->x, this->y, this->width, this->height};

            if (clip != nullptr) {
                renderQuad.w = clip->w;
                renderQuad.h = clip->h;
            }

            Services::Locator::VideoService()->Render(this->texture, clip, &renderQuad);
        }

        void Image::LuaInterface::Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext) {
            //const char LuaGameImage::className[] = "GameImage";
            //
            //Lunar<LuaGameImage>::RegType LuaGameImage::methods[] = {
            //    {"getX", &LuaGameImage::getX},
            //    {"getY", &LuaGameImage::getY},
            //    {"getWidth", &LuaGameImage::getWidth},
            //    {"getHeight", &LuaGameImage::getHeight},
            //    {"setPosition", &LuaGameImage::setPosition},
            //    {"render", &LuaGameImage::render},
            //    {"renderWithClip", &LuaGameImage::renderWithClip},
            //    {0, 0}
            //};

            //class LuaGameImage {
            //public:
            //    static const char className[];
            //    static Lunar<LuaGameImage>::RegType methods[];
            //
            //    LuaGameImage(lua_State *L) {
            //        const int argc = lua_gettop(L);
            //
            //        // GameTexture* texture = nullptr;
            //        std::string textureName = "";
            //        int x = 0;
            //        int y = 0;
            //
            //        if (argc > 0) {
            //            // texture = static_cast<GameTexture*>(lua_touserdata(L, 1));
            //            textureName = luaL_checkstring(L, 1);
            //        }
            //
            //        if (argc > 1) {
            //            x = (int)luaL_checkinteger(L, 2);
            //        }
            //
            //        if (argc > 2) {
            //            y = (int)luaL_checkinteger(L, 3);
            //        }
            //
            //        std::shared_ptr<GameTexture> texture = Services::Locator::TextureService()->GetTexture(textureName);
            //
            //        this->realObject = new GameImage(texture, x, y);
            //    }
            //
            //    ~LuaGameImage() {
            //        delete this->realObject;
            //    }
            //
            //    int getX(lua_State *L) {
            //        SDL_Rect position = this->realObject->GetPosition();
            //
            //        lua_pushnumber(L, position.x);
            //
            //        return 1;
            //    }
            //
            //    int getY(lua_State *L) {
            //        SDL_Rect position = this->realObject->GetPosition();
            //
            //        lua_pushnumber(L, position.y);
            //
            //        return 1;
            //    }
            //
            //    int getWidth(lua_State *L) {
            //        SDL_Rect position = this->realObject->GetPosition();
            //
            //        lua_pushnumber(L, position.w);
            //
            //        return 1;
            //    }
            //
            //    int getHeight(lua_State *L) {
            //        SDL_Rect position = this->realObject->GetPosition();
            //
            //        lua_pushnumber(L, position.h);
            //
            //        return 1;
            //    }
            //
            //    int setPosition(lua_State *L) {
            //        int x = (int)luaL_checkinteger(L, 1);
            //
            //        int y = (int)luaL_checkinteger(L, 2);
            //
            //        this->realObject->SetPosition(x, y);
            //
            //        return 0;
            //    }
            //
            //    int render(lua_State *L) {
            //        this->realObject->Render();
            //
            //        return 0;
            //    }
            //
            //    int renderWithClip(lua_State *L) {
            //        int x = luaL_checkint(L, 1);
            //        int y = luaL_checkint(L, 2);
            //        int w = luaL_checkint(L, 3);
            //        int h = luaL_checkint(L, 4);
            //
            //        SDL_Rect clip = {x, y, w, h};
            //
            //        this->realObject->Render(&clip);
            //
            //        return 0;
            //    }
            //private:
            //    GameImage* realObject;
            //};

        }
    }
}
