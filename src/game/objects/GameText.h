#ifndef __SDL03__GameText__
#define __SDL03__GameText__

#include <string>
#include <iostream>

#include "GameFont.h"
#include "GameObject.h"

class GameEngine;

class GameText : public GameObject {
public:
    GameText();
    GameText(std::string text, GameFont* font, int x, int y, SDL_Color color);
    ~GameText();
    std::string GetText();
    void SetText(std::string text);
    void SetFont(GameFont* font);
    SDL_Rect GetPosition();
    void SetPosition(int x, int y);
    SDL_Color GetColor();
    void SetColor(SDL_Color color);
    void Render();
private:
    std::string text;
    GameFont* font;
    int x;
    int y;
    SDL_Color color;
    SDL_Rect textLocation;
    SDL_Texture *texture;

    void UpdateTexture();
};

class LuaGameText {
public:
    static const char className[];
    static Lunar<LuaGameText>::RegType methods[];

    LuaGameText(lua_State *L) {
        int argc = lua_gettop(L);

        std::string text;

        GameFont* font;

        int x;

        int y;

        Uint8 red;

        Uint8 green;

        Uint8 blue;

        SDL_Color color;

        if (argc > 0) {
            text = luaL_checkstring(L, 1);
        } else {
            text = "";
        }

        if (argc > 1) {
            font = (GameFont*)lua_touserdata(L, 2);
        } else {
            font = NULL;
        }

        if (argc > 2) {
            x = (int)luaL_checkinteger(L, 3);
        } else {
            x = 0;
        }

        if (argc > 3) {
            y = (int)luaL_checkinteger(L, 4);
        } else {
            y = 0;
        }

        if (argc > 4) {
            red = (Uint8)luaL_checkinteger(L, 5);
        } else {
            red = 255;
        }

        if (argc > 5) {
            green = (Uint8)luaL_checkinteger(L, 6);
        } else {
            green = 255;
        }

        if (argc > 6) {
            blue = (Uint8)luaL_checkinteger(L, 7);
        } else {
            blue = 255;
        }

        color = {red, green, blue};

        this->realObject = new GameText(text, font, x, y, color);
    }

    ~LuaGameText() {
        delete this->realObject;
    }

    int getText(lua_State* L) {
        lua_pushstring(L, this->realObject->GetText().c_str());

        return 1;
    }

    int setText(lua_State *L) {
        std::string newText = luaL_checkstring(L, 1);

        this->realObject->SetText(newText);

        return 0;
    }

//    int setFont(lua_State *L) {
//
//        return 0;
//    }

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

    int setColor(lua_State *L) {
        Uint8 red = (Uint8)luaL_checkinteger(L, 1);

        Uint8 green = (Uint8)luaL_checkinteger(L, 1);

        Uint8 blue = (Uint8)luaL_checkinteger(L, 1);

        SDL_Color color = {red, green, blue};

        this->realObject->SetColor(color);

        return 0;
    }

    int render(lua_State *L) {
        this->realObject->Render();

        return 0;
    }
private:
    GameText* realObject;
};

#endif
