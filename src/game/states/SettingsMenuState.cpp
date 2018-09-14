#include "SettingsMenuState.h"

const char LuaSettingsMenuState::className[] = "SettingsMenuState";

Lunar<LuaSettingsMenuState>::RegType LuaSettingsMenuState::methods[] = {
    {"pop", &LuaSettingsMenuState::pop},
    {"getTexture", &LuaGameState::getTexture},
    {"getFont", &LuaGameState::getFont},
    {"getSound", &LuaGameState::getSound},
    {"loadTexture", &LuaGameState::loadTexture},
    {"enableRawInput", &LuaSettingsMenuState::enableRawInput},
    {"disableRawInput", &LuaSettingsMenuState::disableRawInput},
    {0, 0}
};

SettingsMenuState::SettingsMenuState(std::function<void(GameState*)> callback) : GameState(callback) {
    this->logger = new Log::Logger("states.settings_menu");

    this->LoadResources("resources/asset_lits/settings_menu_textures.json", "resources/asset_lits/settings_menu_sounds.json");

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

    Lunar<LuaSettingsMenuState>::Register(this->luaState);

    lua_pushlightuserdata(this->luaState, (void*)this);

    lua_setglobal(this->luaState, "raw_settings_menu_state");

    lua_settop(this->luaState, 0);

    std::cout << "Loading settings_menu.lua" << std::endl;

    if (luaL_loadfile(this->luaState, "scripts/states/settings_menu.lua")) {
        std::cerr << "Error: " << lua_tostring(this->luaState, -1) << std::endl;

        lua_pop(this->luaState, 1);
    } else {
        std::cout << "Loaded settings_menu.lua" << std::endl;
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

SettingsMenuState::~SettingsMenuState() {
    if (this->luaState) {
        lua_close(this->luaState);
    }
}

GameState* SettingsMenuState::Update(int key) {
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

    return this;
}

GameState* SettingsMenuState::Update(SDL_Event* event) {
    if (event) {
        if (event->type == SDL_KEYDOWN) {
            this->ProcessInput(event->key.keysym.sym);
        }
    }

    lua_getglobal(this->luaState, "update");

    if (lua_pcall(this->luaState, 0, 0, 0)) {
        std::cerr << "Error: " << lua_tostring(this->luaState, -1) << std::endl;

        lua_pop(this->luaState, 1);
    }

    return this;
}

std::string SettingsMenuState::ProcessInput(int key) {
    lua_getglobal(this->luaState, "process_input");

    lua_pushinteger(this->luaState, key);

    if (lua_pcall(this->luaState, 1, 1, 0)) {
        std::cerr << "Error: " << lua_tostring(this->luaState, -1) << std::endl;

        lua_pop(this->luaState, 1);
    }

    std::string result = "";

    if (!lua_isstring(this->luaState, -1)) {
        std::cerr << "Error: expected process_input to return a string." << std::endl;
    } else {
        result = lua_tostring(this->luaState, -1);
    }

    lua_pop(this->luaState, 1);

    return result;
}

void SettingsMenuState::Render() {
    lua_getglobal(this->luaState, "render");
    
    if (lua_pcall(this->luaState, 0, 0, 0)) {
        std::cerr << "Error: " << lua_tostring(this->luaState, -1) << std::endl;
        
        lua_pop(this->luaState, 1);
    }
}
