#include "IntroState.h"

#include <iostream>
#include <sstream>

IntroState::IntroState(SDL_Renderer* renderer) : GameState(renderer) {
    this->renderer = renderer;

    std::cout << "Creating new Lua state" << std::endl;
    this->luaState = luaL_newstate();

    std::cout << "Opening Lua libs" << std::endl;
    luaL_openlibs(this->luaState);

    std::cout << "Loading intro.lua" << std::endl;
    if (luaL_loadfile(this->luaState, "intro.lua")) {
        std::cerr << "Error loading intro.lua" << std::endl;

        std::cerr << lua_tostring(this->luaState, -1) << std::endl;

        std::cerr << "Popping error message off the Lua stack" << std::endl;
        lua_pop(this->luaState, 1);
    } else {
        std::cout << "Loaded intro.lua" << std::endl;
    }

    std::cout << "Pushing 0.0 onto Lua stack" << std::endl;
    lua_pushnumber(this->luaState, 0.0);

    std::cout << "Assigning pushed number to global currentTime" << std::endl;
    lua_setglobal(this->luaState, "currentTime");

    std::cout << "Executing intro.lua" << std::endl;
    if (lua_pcall(this->luaState, 0, LUA_MULTRET, 0)) {
        std::cerr << "Error executing intro.lua" << std::endl;

        std::cerr << lua_tostring(this->luaState, -1) << std::endl;

        std::cerr << "Popping error message off the Lua stack" << std::endl;
        lua_pop(this->luaState,1);
    }

    this->pop = false;
}

IntroState::~IntroState() {
    if (this->luaState) {
        lua_close(this->luaState);
    }
}

GameState* IntroState::Update(SDL_Event* event) {
    if (event) {
        if (event->type == SDL_KEYDOWN || event->type == SDL_MOUSEBUTTONDOWN) {
            this->ProcessInput(event);
        }
    }

    if (this->pop) {
        return NULL;
    } else {
        return this;
    }
}

void IntroState::ProcessInput(SDL_Event* event) {
    if (event->type == SDL_KEYDOWN) {
        this->pop = true;
    }
}

// This needs to be completely redone. Possibly by having each state have its own
// lists of resources that are loaded when transitioning to the state. This should
// avoid having to pass in everything awkwardly. It would however greatly increase
// the likelyhood of needing to reload resouces multiple times, especially textures
// and fonts.
void IntroState::RenderObjects(std::map <std::string, GameTexture*> textures, std::map <std::string, GameFont*> fonts, std::map <std::string, GameSound*> sounds) {
    SDL_Texture *textTexture = NULL;

    SDL_Color color = { 255, 255, 255 };

    SDL_Rect textLocation;

    static int i = 0;

    i++;

    if (i > 60) {
        i = 0;

        std::cout << "Reseting frame counter" << std::endl;
    }

    if (i == 0) {
        std::cout << "Getting Lua function incrementTime" << std::endl;
        lua_getglobal(this->luaState, "incrementTime");

        std::cout << "Pushing 1.0 onto Lua stack" << std::endl;
        lua_pushnumber(this->luaState, 1.0);

        std::cout << "Calling Lua function incrementTime" << std::endl;
        lua_pcall(this->luaState, 1, 1, 0);

        std::cout << "The return value of the incrementTime was " << lua_tostring(this->luaState, -1) << std::endl;

        std::cout << "Popping return value of incrementTime off the Lua stack" << std::endl;
        lua_pop(this->luaState, 1);
    }

    //std::cout << "Getting Lua global currentTime" << std::endl;
    lua_getglobal(this->luaState, "currentTime");

    //std::cout << "Reading Lua global currentTime from Lua stack" << std::endl;
    double currentTime = lua_tonumber(this->luaState, -1);

    //std::cout << "Popping Lua global currentTime off the sLua tack" << std::endl;
    lua_pop(this->luaState, 1);

    std::stringstream s;

    s << currentTime;

    SDL_Surface *textSurface = TTF_RenderText_Blended(fonts["DroidSans"]->font, s.str().c_str(), color);

    textTexture = SDL_CreateTextureFromSurface(this->renderer, textSurface);

    textLocation.h = textSurface->h;

    textLocation.w = textSurface->w;

    textLocation.x = 100;

    textLocation.y = 100;

    SDL_FreeSurface(textSurface);

    SDL_RenderCopy(this->renderer, textTexture, NULL, &textLocation);

    // Need to figure out how to store textures as objects or something since this is
    // creating a new texture on every pass which is very inefficient.
    SDL_DestroyTexture(textTexture);
}
