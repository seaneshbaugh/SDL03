#ifndef __SDL03__GameImage__
#define __SDL03__GameImage__

#include "GameTexture.h"
#include "GameObject.h"

// This class is for generic things that are in the game but are really only used
// for displaying an image by itself not backed by any real data. Some of the
// things this is used for may be rolled into other types later on.
class GameImage : public GameObject {
public:
    static SDL_Renderer* renderer;

    GameImage();
    GameImage(GameTexture* texture, int x, int y);
    ~GameImage();
    void SetTexture(GameTexture* texture);
    SDL_Rect GetPosition();
    void SetPosition(int x, int y);
    void Render(SDL_Rect* clip = nullptr);
private:
    GameTexture* texture;
    int x;
    int y;
    int width;
    int height;
    SDL_Rect textureLocation;

    void UpdateTexture();
};

class LuaGameImage {
public:
    static const char className[];
    static Lunar<LuaGameImage>::RegType methods[];

    LuaGameImage(lua_State *L) {
        int argc = lua_gettop(L);

        GameTexture* texture;

        int x;

        int y;

        if (argc > 0) {
            texture = (GameTexture*)lua_touserdata(L, 1);
        } else {
            texture = NULL;
        }

        if (argc > 1) {
            x = (int)luaL_checkinteger(L, 2);
        } else {
            x = 0;
        }

        if (argc > 2) {
            y = (int)luaL_checkinteger(L, 3);
        } else {
            y = 0;
        }

        this->realObject = new GameImage(texture, x, y);
    }

    ~LuaGameImage() {
        delete this->realObject;
    }

    int getX(lua_State *L) {
        SDL_Rect position = this->realObject->GetPosition();

        lua_pushnumber(L, position.x);

        return 1;
    }

    int getY(lua_State *L) {
        SDL_Rect position = this->realObject->GetPosition();

        lua_pushnumber(L, position.y);

        return 1;
    }

    int getWidth(lua_State *L) {
        SDL_Rect position = this->realObject->GetPosition();

        lua_pushnumber(L, position.w);

        return 1;
    }

    int getHeight(lua_State *L) {
        SDL_Rect position = this->realObject->GetPosition();

        lua_pushnumber(L, position.h);

        return 1;
    }

    int setPosition(lua_State *L) {
        int x = (int)luaL_checkinteger(L, 1);

        int y = (int)luaL_checkinteger(L, 2);

        this->realObject->SetPosition(x, y);

        return 0;
    }

    int render(lua_State *L) {
        this->realObject->Render();
        
        return 0;
    }

    int renderWithClip(lua_State *L) {
        int x = luaL_checkint(L, 1);
        int y = luaL_checkint(L, 2);
        int w = luaL_checkint(L, 3);
        int h = luaL_checkint(L, 4);

        SDL_Rect clip = {x, y, w, h};

        this->realObject->Render(&clip);

        return 0;
    }
private:
    GameImage* realObject;
};

#endif
