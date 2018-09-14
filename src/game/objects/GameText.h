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
    GameText(std::string text, std::shared_ptr<GameFont> font, int x, int y, SDL_Color color);
    ~GameText();
    std::string GetText();
    void SetText(std::string text);
    void SetFont(std::shared_ptr<GameFont> font);
    SDL_Rect GetPosition();
    void SetPosition(int x, int y);
    SDL_Color GetColor();
    void SetColor(SDL_Color color);
    void Render();
private:
    std::string text;
    std::shared_ptr<GameFont> font;
    int x;
    int y;
    SDL_Color color;
    SDL_Rect textLocation;
    SDL_Texture* texture;

    void UpdateTexture();
};

class LuaGameText {
public:
    static const char className[];
    static Lunar<LuaGameText>::RegType methods[];

    LuaGameText(lua_State *L) {
        const int argc = lua_gettop(L);

        std::string text = "";
        std::string fontName = GameFont::DEFAULT_FONT_NAME;
        int fontSize = GameFont::DEFAULT_FONT_SIZE;
        int x = 0;
        int y = 0;
        Uint8 red = 255;
        Uint8 green = 255;
        Uint8 blue = 255;

        if (argc > 0) {
            text = luaL_checkstring(L, 1);
        }

        if (argc > 1) {
            fontName = luaL_checkstring(L, 2);
        }

        if (argc > 2) {
            fontSize = luaL_checkint(L, 3);
        }

        if (argc > 3) {
            x = luaL_checkint(L, 4);
        }

        if (argc > 4) {
            y = luaL_checkint(L, 5);
        }

        if (argc > 5) {
            red = static_cast<Uint8>(luaL_checkinteger(L, 6));
        }

        if (argc > 6) {
            green = static_cast<Uint8>(luaL_checkinteger(L, 7));
        }

        if (argc > 7) {
            blue = static_cast<Uint8>(luaL_checkinteger(L, 8));
        }

        const SDL_Color color = {red, green, blue};

        const std::shared_ptr<GameFont> font = Services::Locator::FontService()->GetFont(fontName, fontSize);

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
