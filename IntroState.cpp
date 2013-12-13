#include "IntroState.h"

IntroState::IntroState(SDL_Renderer* renderer) : GameState(renderer) {
    this->renderer = renderer;

    this->LoadResources("intro_textures.json", "fonts.json", "intro_sounds.json");

    //this->texts.push_back(new GameText("Hello, world!", this->fonts["DroidSans"], 100, 100, {255, 0, 0}));

    this->luaState = luaL_newstate();

    luaL_openlibs(this->luaState);

    luaopen_base(this->luaState);
    luaopen_table(this->luaState);
    luaopen_io(this->luaState);
    luaopen_string(this->luaState);
    luaopen_math(this->luaState);
    luaopen_debug(this->luaState);

    Lunar<LuaGameState>::Register(this->luaState);

    Lunar<LuaGameText>::Register(this->luaState);

    lua_pushlightuserdata(this->luaState, (void*)this);

    lua_setglobal(this->luaState, "raw_intro_state");

    lua_settop(this->luaState, 0);

    std::cout << "Loading intro.lua" << std::endl;
    if (luaL_loadfile(this->luaState, "intro.lua")) {
        std::cerr << "Error: " << lua_tostring(this->luaState, -1) << std::endl;

        lua_pop(this->luaState, 1);
    } else {
        std::cout << "Loaded intro.lua" << std::endl;
    }

    if (lua_pcall(this->luaState, 0, LUA_MULTRET, 0)) {
        std::cerr << "Error: " << lua_tostring(this->luaState, -1) << std::endl;

        lua_pop(this->luaState, 1);
    }

    lua_getglobal(this->luaState, "initialize");

    if (lua_pcall(this->luaState, 0, LUA_MULTRET, 0)) {
        std::cerr << "Error: " << lua_tostring(this->luaState, -1) << std::endl;

        lua_pop(this->luaState, 1);
    }

    this->pop = false;
}

IntroState::~IntroState() {
    if (this->luaState) {
        lua_close(this->luaState);
    }
}

GameState* IntroState::Update(SDL_Event* event) {
    std::string nextState = "";

    if (event) {
        if (event->type == SDL_KEYDOWN || event->type == SDL_MOUSEBUTTONDOWN) {
            nextState = this->ProcessInput(event);
        }
    }

    lua_getglobal(this->luaState, "update");

    if (lua_pcall(this->luaState, 0, 0, 0)) {
        std::cerr << "Error: " << lua_tostring(this->luaState, -1) << std::endl;

        lua_pop(this->luaState, 1);
    }

    // I hate this but C++ doesn't let you do switch statements for strings. Whatever.
    if (this->pop) {
        return NULL;
    }

    if (nextState == "main_menu") {
        return new MainMenuState(this->renderer);
    }

    return this;
}

// Right now the Lua proccess_input function does nothing. Eventually the C++
// function will take the raw SDL event and then pass along something to the Lua
// function to indicate what happened.
std::string IntroState::ProcessInput(SDL_Event* event) {
    lua_getglobal(this->luaState, "process_input");

//    lua_pushinteger(this->luaState, event->key.keysym.sym);

    if (lua_pcall(this->luaState, 0, 1, 0)) {
        std::cerr << "Error: " << lua_tostring(this->luaState, -1) << std::endl;

        lua_pop(this->luaState, 1);
    }

    // This is probably a really bad idea. The Lua process_input function returns a
    // string that should be the name of the state it should go to (if any).
    // Now that I think about it I'm probably just going make it so the main processing
    // of input happens in C++ and then only input relevant to a given state is passed
    // down to Lua.
    // The intro state should just go to the next state immediately upon any keyboard
    // input.
    std::string result = "";

    if (!lua_isstring(this->luaState, -1)) {
        std::cerr << "Error: " << "expected process_input to return a string." << std::endl;
    } else {
        result = lua_tostring(this->luaState, -1);
    }

    lua_pop(this->luaState, 1);

    if (event->type == SDL_KEYDOWN) {
//        this->pop = true;

        result = "main_menu";
    }

    return result;
}

void IntroState::Render() {
    lua_getglobal(this->luaState, "render");

    if (lua_pcall(this->luaState, 0, 0, 0)) {
        std::cerr << "Error: " << lua_tostring(this->luaState, -1) << std::endl;

        lua_pop(this->luaState, 1);
    }

    // For now the C++ object has its own array of text objects. Eventually this should
    // go away since I'd really rather keep as much of the game objects in Lua. I'm
    // keeping this here just to sort of show it's possible.
    for (int i = 0; i < this->texts.size(); i++) {
        this->texts[i]->Render();
    }
}
