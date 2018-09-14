#include "MapState.h"

const char LuaMapState::className[] = "MapState";

Lunar<LuaMapState>::RegType LuaMapState::methods[] = {
    {"pop", &LuaMapState::pop},
    {"getTexture", &LuaMapState::getTexture},
    {"getFont", &LuaMapState::getFont},
    {"getSound", &LuaMapState::getSound},
    {"loadMap", &LuaMapState::loadMap},
    {"getCurrentMap", &LuaMapState::getCurrentMap},
    {"loadTexture", &LuaMapState::loadTexture},
    {"setCurrentEncounterArea", &LuaMapState::setCurrentEncounterArea},
    {"getPlayerSpriteName", &LuaMapState::getPlayerSpriteName},
    {0, 0}
};

MapState::MapState(std::function<void(GameState*)> callback) : GameState(callback) {
    this->logger = new Log::Logger("states.map");

    this->currentMap = GameState::world->currentMap;

    this->luaState = luaL_newstate();

    luaL_openlibs(this->luaState);

    luaopen_base(this->luaState);
    luaopen_table(this->luaState);
    luaopen_io(this->luaState);
    luaopen_string(this->luaState);
    luaopen_math(this->luaState);
    luaopen_debug(this->luaState);

    Lunar<LuaGameState>::Register(this->luaState);

    Lunar<LuaMapState>::Register(this->luaState);

    Lunar<LuaGameText>::Register(this->luaState);

    Lunar<LuaGameImage>::Register(this->luaState);

    Lunar<LuaGameMap>::Register(this->luaState);

    Lunar<LuaGameMapObject>::Register(this->luaState);

    lua_pushlightuserdata(this->luaState, (void*)this);

    lua_setglobal(this->luaState, "raw_map_state");

    lua_settop(this->luaState, 0);

    this->logger->info() << "Loading \"scripts/states/map.lua\"";

    if (luaL_loadfile(this->luaState, "scripts/states/map.lua")) {
        this->logger->error() << lua_tostring(this->luaState, -1);

        lua_pop(this->luaState, 1);
    } else {
        this->logger->info() << "Loaded \"scripts/states/map.lua\"";
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

    this->currentEncounterArea = nullptr;

    if (callback) {
        callback(this);
    }
}

MapState::~MapState() {
    if (this->luaState) {
        lua_close(this->luaState);
    }

    if (this->currentMap) {
        delete currentMap;
    }
}

GameState* MapState::Update(int key) {
    std::string nextState = "";

    if (key != NO_KEY) {
        this->ProcessInput(key);
    }

    lua_getglobal(this->luaState, "update");

    if (lua_pcall(this->luaState, 0, 1, 0)) {
        this->logger->error() << lua_tostring(this->luaState, -1);

        lua_pop(this->luaState, 1);
    }

    if (!lua_isstring(this->luaState, -1)) {
        this->logger->error() << "Error: expected update to return a string.";
    } else {
        nextState = lua_tostring(this->luaState, -1);
    }

    if (this->pop) {
        return nullptr;
    }

    if (nextState == "battle") {
        return new BattleState(this->currentEncounterArea, nullptr);
    }

    return this;
}

std::string MapState::ProcessInput(int key) {
    this->logger->debug() << "MapState::ProcessInput : key = " << key;

    lua_getglobal(this->luaState, "process_input");

    lua_pushinteger(this->luaState, key);

    if (lua_pcall(this->luaState, 1, 1, 0)) {
        this->logger->error() << lua_tostring(this->luaState, -1);

        lua_pop(this->luaState, 1);
    }

    // This is probably a really bad idea. The Lua process_input function returns a
    // string that should be the name of the state it should go to (if any).
    // Now that I think about it I'm probably just going make it so the main processing
    // of input happens in C++ and then only input relevant to a given state is passed
    // down to Lua.
    // For example: the intro state should just go to the next state immediately upon
    // any keyboard input without bothering to go to the lua process_input function.
    std::string result = "";

    if (!lua_isstring(this->luaState, -1)) {
        this->logger->error() << "Error: expected process_input to return a string.";
    } else {
        result = lua_tostring(this->luaState, -1);
    }

    lua_pop(this->luaState, 1);

    return result;
}

void MapState::Render() {
    lua_getglobal(this->luaState, "render");

    if (lua_pcall(this->luaState, 0, 0, 0)) {
        this->logger->error() << lua_tostring(this->luaState, -1);

        lua_pop(this->luaState, 1);
    }
}

bool MapState::LoadMap(std::string filename) {
    if (!GameState::world->LoadMap(filename)) {
        return false;
    }

    this->currentMap = GameState::world->currentMap;

    lua_getglobal(this->luaState, "after_map_load");

    if (lua_pcall(this->luaState, 0, LUA_MULTRET, 0)) {
        this->logger->error() << lua_tostring(this->luaState, -1);

        lua_pop(this->luaState, 1);
    }

    return true;
}

bool MapState::UnloadMap() {
    GameState::world->UnloadMap();

    this->currentMap = GameState::world->currentMap;

    if (this->currentMap) {
        return false;
    }

    lua_getglobal(this->luaState, "after_map_unload");

    if (lua_pcall(this->luaState, 0, LUA_MULTRET, 0)) {
        this->logger->error() << lua_tostring(this->luaState, -1);

        lua_pop(this->luaState, 1);
    }

    return false;
}
