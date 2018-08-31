#include "MainMenuState.h"

MainMenuState::MainMenuState(std::function<void(GameState*)> callback) : GameState(callback) {
    this->logger = new Log::Logger("states.main_menu");

    this->LoadResources("resources/asset_lists/main_menu_textures.json", "resources/asset_lists/fonts.json", "resources/asset_lists/main_menu_sounds.json");

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

    Lunar<LuaGameImage>::Register(this->luaState);

    lua_pushlightuserdata(this->luaState, (void*)this);

    lua_setglobal(this->luaState, "raw_main_menu_state");

    lua_settop(this->luaState, 0);

    std::cout << "Loading main_menu.lua" << std::endl;

    if (luaL_loadfile(this->luaState, "scripts/states/main_menu.lua")) {
        std::cerr << "Error: " << lua_tostring(this->luaState, -1) << std::endl;

        lua_pop(this->luaState, 1);
    } else {
        std::cout << "Loaded main_menu.lua" << std::endl;
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

    this->acceptRawInput = false;

    if (callback) {
        callback(this);
    }
}

MainMenuState::~MainMenuState() {
    if (this->luaState) {
        lua_close(this->luaState);
    }
}

GameState* MainMenuState::Update(int key) {
    std::string nextState = "";

    if (key != NO_KEY) {
        nextState = this->ProcessInput(key);
    }

    lua_getglobal(this->luaState, "update");

    if (lua_pcall(this->luaState, 0, 0, 0)) {
        std::cerr << "Error: " << lua_tostring(this->luaState, -1) << std::endl;

        lua_pop(this->luaState, 1);
    }

    if (this->pop) {
        return nullptr;
    }

    if (nextState == "new_game") {
        this->logger->info() << "new_game";

        return GameState::NewGame();
    }

    // Switch to the load game menu.
    if (nextState == "load_game") {
        this->logger->info() << "load_game";

        return this;
    }

    // Switch to the settings menu.
    if (nextState == "settings") {
        std::function<void(GameState*)> callback = [] (GameState* nextGameState) {
            std::cout << "settings" << std::endl;
        };

        return new SettingsMenuState(callback);
    }

    return this;
}

std::string MainMenuState::ProcessInput(int key) {
    lua_getglobal(this->luaState, "process_input");

    lua_pushinteger(this->luaState, key);

    if (lua_pcall(this->luaState, 1, 1, 0)) {
        std::cerr << "Error: " << lua_tostring(this->luaState, -1) << std::endl;

        lua_pop(this->luaState, 1);
    }

    // This is probably a really bad idea. The Lua process_input function returns a
    // string that should be the name of the state it should go to (if any).
    // Now that I think about it I'm probably just going make it so the main processing
    // of input happens in C++ and then only input relevant to a given state is passed
    // down to Lua.
    std::string result = "";

    if (!lua_isstring(this->luaState, -1)) {
        std::cerr << "Error: " << "expected process_input to return a string." << std::endl;
    } else {
        result = lua_tostring(this->luaState, -1);
    }

    lua_pop(this->luaState, 1);

    return result;
}

void MainMenuState::Render() {
    lua_getglobal(this->luaState, "render");

    if (lua_pcall(this->luaState, 0, 0, 0)) {
        std::cerr << "Error: " << lua_tostring(this->luaState, -1) << std::endl;

        lua_pop(this->luaState, 1);
    }
}
