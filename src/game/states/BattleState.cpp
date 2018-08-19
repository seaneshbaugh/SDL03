#include "BattleState.h"

const char LuaBattleState::className[] = "BattleState";

Lunar<LuaBattleState>::RegType LuaBattleState::methods[] = {
    {"pop", &LuaBattleState::pop},
    {"getTexture", &LuaBattleState::getTexture},
    {"getFont", &LuaBattleState::getFont},
    {"getSound", &LuaBattleState::getSound},
    {"loadTexture", &LuaBattleState::loadTexture},
    {"getParty", &LuaBattleState::getParty},
    {"getMonsters", &LuaBattleState::getMonsters},
    {0, 0}
};

BattleState::BattleState(std::function<void(GameState*)> callback) : GameState(callback) {
    this->LoadResources("battle_textures.json", "fonts.json", "battle_sounds.json");

    this->luaState = luaL_newstate();

    luaL_openlibs(this->luaState);

    luaopen_base(this->luaState);
    luaopen_table(this->luaState);
    luaopen_io(this->luaState);
    luaopen_string(this->luaState);
    luaopen_math(this->luaState);
    luaopen_debug(this->luaState);

    Lunar<LuaGameState>::Register(this->luaState);

    Lunar<LuaBattleState>::Register(this->luaState);

    Lunar<LuaGameText>::Register(this->luaState);

    Lunar<LuaGameImage>::Register(this->luaState);

    Lunar<LuaGameCharacter>::Register(this->luaState);

    Lunar<LuaGameMonster>::Register(this->luaState);

    lua_pushlightuserdata(this->luaState, (void*)this);

    lua_setglobal(this->luaState, "raw_battle_state");

    lua_settop(this->luaState, 0);

    std::cout << "Loading battle.lua" << std::endl;

    if (luaL_loadfile(this->luaState, "battle.lua")) {
        std::cerr << "Error: " << lua_tostring(this->luaState, -1) << std::endl;

        lua_pop(this->luaState, 1);
    } else {
        std::cout << "Loaded battle.lua" << std::endl;
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

BattleState::~BattleState() {
    if (this->luaState) {
        lua_gc(this->luaState, LUA_GCCOLLECT, 0);

        lua_close(this->luaState);
    }

    for (std::vector<GameMonster*>::iterator monster = this->monsters.begin(); monster != this->monsters.end(); monster++) {
        delete (*monster);
    }
}

GameState* BattleState::Update(int key) {
    std::string nextState = "";

    if (key != NO_KEY) {
        this->ProcessInput(key);
    }

    lua_getglobal(this->luaState, "update");

    if (lua_pcall(this->luaState, 0, 1, 0)) {
        std::cerr << "Error: " << lua_tostring(this->luaState, -1) << std::endl;

        lua_pop(this->luaState, 1);
    }

    if (!lua_isstring(this->luaState, -1)) {
        std::cerr << "Error: expected update to return a string." << std::endl;
    } else {
        nextState = lua_tostring(this->luaState, -1);
    }

    if (this->pop) {
        return nullptr;
    }

    return this;
}

std::string BattleState::ProcessInput(int key) {
    lua_getglobal(this->luaState, "process_input");

    lua_pushinteger(this->luaState, key);

    if (lua_pcall(this->luaState, 1, 1, 0)) {
        std::cerr << "Error: " << lua_tostring(this->luaState, -1) << std::endl;

        lua_pop(this->luaState, 1);
    }

    std::string result = "";

    if (!lua_isstring(this->luaState, -1)) {
        std::cerr << "Error: " << "expected process_input to return a string." << std::endl;
    } else {
        result = lua_tostring(this->luaState, -1);
    }

    lua_pop(this->luaState, 1);

    return result;
}

void BattleState::Render() {
    lua_getglobal(this->luaState, "render");
    
    if (lua_pcall(this->luaState, 0, 0, 0)) {
        std::cerr << "Error: " << lua_tostring(this->luaState, -1) << std::endl;
        
        lua_pop(this->luaState, 1);
    }
}