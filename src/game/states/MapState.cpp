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
    {0, 0}
};

MapState::MapState(std::function<void(GameState*)> callback) : GameState(callback) {
    // We only load the fonts here because the textures and sounds that will be loaded
    // are determined by the map file.
    this->LoadFonts("resources/asset_lists/fonts.json");

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

    std::cout << "Loading map.lua" << std::endl;
    if (luaL_loadfile(this->luaState, "scripts/states/map.lua")) {
        std::cerr << "Error: " << lua_tostring(this->luaState, -1) << std::endl;

        lua_pop(this->luaState, 1);
    } else {
        std::cout << "Loaded map.lua" << std::endl;
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

    this->currentMap = nullptr;

    this->currentEncounterArea = nullptr;

    // This is where any initial maps should be loaded.
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

    // TODO: Make all of this less hard coded and move it someplace that makes more sense.
    if (nextState == "battle") {
        std::function<void(GameState*)> callback = [&] (GameState* nextGameState) {
            BattleState* battleState = static_cast<BattleState*>(nextGameState);

            GameTexture* background = new GameTexture();

            background->Load(this->currentEncounterArea->properties["background"]);

            battleState->textures["background"] = background;

            std::random_device rd;

            std::mt19937 mt(rd());

            std::uniform_int_distribution<int> dist(1, 6);

            int numberOfMonsters = dist(mt);

            for (int i = 0; i < numberOfMonsters; i++) {
                GameMonster* monster = new GameMonster();

                monster->Load("resources/monsters/slime.json");

                battleState->monsters.push_back(monster);
            }

            lua_getglobal(battleState->luaState, "after_battle_load");

            if (lua_pcall(battleState->luaState, 0, LUA_MULTRET, 0)) {
                std::cerr << "Error: " << lua_tostring(battleState->luaState, -1) << std::endl;

                lua_pop(battleState->luaState, 1);
            }
        };
        
        return new BattleState(callback);
    }

    return this;
}

std::string MapState::ProcessInput(int key) {
    std::cout << "MapState::ProcessInput : key = " << key << std::endl;
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
        std::cerr << "Error: expected process_input to return a string." << std::endl;
    } else {
        result = lua_tostring(this->luaState, -1);
    }

    lua_pop(this->luaState, 1);

    return result;
}

void MapState::Render() {
    lua_getglobal(this->luaState, "render");

    if (lua_pcall(this->luaState, 0, 0, 0)) {
        std::cerr << "Error: " << lua_tostring(this->luaState, -1) << std::endl;

        lua_pop(this->luaState, 1);
    }
}

bool MapState::LoadMap(std::string filename) {
    this->UnloadMap();

    this->currentMap = new GameMap();

    if (this->currentMap->Load(filename)) {
        return true;
    } else {
        return false;
    }
}

bool MapState::UnloadMap() {
    if (this->currentMap) {
        delete this->currentMap;
        
        return true;
    } else {
        return false;
    }
}
