#include "IntroState.h"

IntroState::IntroState(std::function<void(GameState*)> callback) : GameState(callback) {
    this->logger = new Log::Logger("states.intro");

    // this->fontSizes["DroidSans"] = 16;

    this->LoadResources("resources/asset_lists/intro_textures.json", "resources/asset_lists/intro_sounds.json");

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

    this->logger->debug() << "Loading \"intro.lua\".";

    if (luaL_loadfile(this->luaState, "scripts/states/intro.lua")) {
        this->logger->error() << lua_tostring(this->luaState, -1);

        lua_pop(this->luaState, 1);
    } else {
        this->logger->debug() << "Loaded \"intro.lua\"";
    }

    if (lua_pcall(this->luaState, 0, LUA_MULTRET, 0)) {
        this->logger->error() << lua_tostring(this->luaState, -1);

        lua_pop(this->luaState, 1);
    }

    lua_getglobal(this->luaState, "initialize");

    if (lua_pcall(this->luaState, 0, LUA_MULTRET, 0)) {
        this->logger->error() << lua_tostring(this->luaState, -1);

        lua_pop(this->luaState, 1);
    }

    this->pop = false;

    this->acceptRawInput = false;

    if (callback) {
        callback(this);
    }
}

IntroState::~IntroState() {
    if (this->luaState) {
        lua_close(this->luaState);
    }
}

GameState* IntroState::Update(int key) {
    std::string nextState = "";

    if (key != NO_KEY) {
        nextState = this->ProcessInput(key);
    }

    lua_getglobal(this->luaState, "update");

    if (lua_pcall(this->luaState, 0, 0, 0)) {
        std::cerr << "Error: " << lua_tostring(this->luaState, -1) << std::endl;

        lua_pop(this->luaState, 1);
    }

    // I hate this but C++ doesn't let you do switch statements for strings. Whatever.
    if (this->pop) {
        return nullptr;
    }

    if (nextState == "main_menu") {
//        std::function<void(GameState*)> callback = [] (GameState* nextGameState) {
//            std::cout << "This is the main menu callback." << std::endl;
//        };

        return new MainMenuState(nullptr);
    }

    return this;
}

// For the intro state the Lua process_input function does nothing. This is because
// any keyboard input just immediately causes a transition to the main menu state.
// Honestly it doesn't even make sense to call the Lua function at all, but I'm
// leaving it in for consistency's sake.
std::string IntroState::ProcessInput(int key) {
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
    // The intro state should just go to the next state immediately upon any keyboard
    // input.
    std::string result = "";

    if (!lua_isstring(this->luaState, -1)) {
        std::cerr << "Error: " << "expected process_input to return a string." << std::endl;
    } else {
        result = lua_tostring(this->luaState, -1);
    }

    lua_pop(this->luaState, 1);

    if (key != NO_KEY) {
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
}
