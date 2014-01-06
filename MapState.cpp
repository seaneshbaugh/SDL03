#include "MapState.h"

const char LuaMapState::className[] = "MapState";

Lunar<LuaMapState>::RegType LuaMapState::methods[] = {
    {"getTexture", &LuaMapState::getTexture},
    {"getFont", &LuaMapState::getFont},
    {"getSound", &LuaMapState::getSound},
    {"loadMap", &LuaMapState::loadMap},
    {"getCurrentMap", &LuaMapState::getCurrentMap},
    {"loadTexture", &LuaMapState::loadTexture},
    {0, 0}
};

MapState::MapState(std::function<void(GameState*)> callback) : GameState(callback) {
    // We only load the fonts here because the textures and sounds that will be loaded
    // are determined by the map file.
    this->LoadFonts("fonts.json");

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
    if (luaL_loadfile(this->luaState, "map.lua")) {
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

    this->currentMap = nullptr;

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

GameState* MapState::Update(SDL_Event* event) {
    if (event) {
        if (event->type == SDL_KEYDOWN || event->type == SDL_MOUSEBUTTONDOWN) {
            this->ProcessInput(event);
        }
    }

    lua_getglobal(this->luaState, "update");

    if (lua_pcall(this->luaState, 0, 0, 0)) {
        std::cerr << "Error: " << lua_tostring(this->luaState, -1) << std::endl;

        lua_pop(this->luaState, 1);
    }

    if (this->pop) {
        return NULL;
    } else {
        return this;
    }
}

std::string MapState::ProcessInput(SDL_Event* event) {
    lua_getglobal(this->luaState, "process_input");

    lua_pushinteger(this->luaState, event->key.keysym.sym);

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
    if (this->currentMap) {
        delete this->currentMap;
    }

    this->currentMap = new GameMap();

    if (this->currentMap->Load(filename)) {
        for (std::map<int, GameMapTile*>::iterator tile = this->currentMap->tiles.begin(); tile != this->currentMap->tiles.end(); tile++) {
            GameTexture* texture = new GameTexture();

            if (!texture->Load(tile->second->filename)) {
                return false;
            }

            size_t extensionIndex = tile->second->filename.find(".png");

            if (extensionIndex != std::string::npos) {
                this->textures[tile->second->filename.substr(0, extensionIndex)] = texture;
            } else {
                this->textures[tile->second->filename] = texture;
            }

            tile->second->texture = texture;
        }

        return true;
    } else {
        return false;
    }
}
