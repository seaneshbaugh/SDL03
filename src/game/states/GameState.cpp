#include "GameState.h"
#include "MapState.h"

// TODO: Move inputMapper to GameEngine?
GameInputMapper* GameState::inputMapper = nullptr;

// I think I want to keep this here since if we don't have a game state
// how could/why would we have a game world?
GameWorld* GameState::world = nullptr;

std::map<std::string, GameStateType> GameState::gameStateTypeMap = {
    {"intro", GameStateType::intro},
    {"main_menu", GameStateType::main_menu},
    {"settings_menu", GameStateType::settings_menu},
    {"map", GameStateType::map},
    {"battle", GameStateType::battle},
    {"pause_menu", GameStateType::pause_menu}
};

// Eventually this will load an initial cutscene. For now it'll just go
// straight to the main "world" map.
MapState* GameState::NewGame() {
    if (GameState::world != nullptr) {
        delete GameState::world;
    }

    GameState::world = new GameWorld();

    return new MapState(nullptr);
}

GameStateType GameState::StateNameToEnum(std::string stateName) {
    return GameState::gameStateTypeMap[stateName];
}

const char LuaGameState::className[] = "GameState";

Lunar<LuaGameState>::RegType LuaGameState::methods[] = {
    {"pop", &LuaGameState::pop},
    {"getTexture", &LuaGameState::getTexture},
    {"getFont", &LuaGameState::getFont},
    {"getSound", &LuaGameState::getSound},
    {"loadTexture", &LuaGameState::loadTexture},
    {0, 0}
};

GameState::GameState(std::function<void(GameState*)> callback) {
    this->pop = false;

    this->acceptRawInput = false;

    if (callback != nullptr) {
        callback(this);
    }
}

GameState::~GameState() {
    for (std::map <std::string, GameTexture*>::iterator it = this->textures.begin(); it != this->textures.end(); it++) {
        delete it->second;
    }

    for (std::map <std::string, GameSound*>::iterator it = this->sounds.begin(); it != this->sounds.end(); it++) {
        delete it->second;
    }
}

GameState* GameState::Update(int key) {
    if (key != NO_KEY) {
        this->ProcessInput(key);
    }

    if (this->pop) {
        return nullptr;
    } else {
        return this;
    }
}

GameState* GameState::Update(SDL_Event* event) {
    if (event != nullptr && event->type == SDL_KEYDOWN) {
        return this->Update(event->key.keysym.sym);
    }

    return this;
}

std::string GameState::ProcessInput(int key) {
    return "";
}

void GameState::Render() {
}

bool GameState::LoadResources(std::string textureListPath, std::string fontListPath, std::string SoundListPath) {
    this->logger->debug() << "Loading resource lists: \"" << textureListPath << "\", \"" << fontListPath << "\", \"" << SoundListPath << "\".";

    return this->LoadTextures(textureListPath) && this->LoadSounds(SoundListPath);
}

bool GameState::LoadTextures(std::string resourceListPath) {
    this->logger->debug() << "Loading textures from \"" << resourceListPath << "\"";

    std::string jsonString;

    if (!FileSystemHelpers::ReadFile(resourceListPath, jsonString)) {
        this->logger->error() << "Error: Failed to load resource list \"" << resourceListPath << "\".";

        return false;
    }

    AssetListParser parser = AssetListParser();
    std::map<std::string, std::string> assetList;
    parser.Parse(jsonString, &assetList);

    this->logger->debug() << "parsed asset list contents = {";
    for (std::map<std::string, std::string>::iterator i = assetList.begin(); i != assetList.end(); i++) {
        this->logger->debug() << (*i).first << " : " << (*i).second;
    }
    this->logger->debug() << "}";

    for (std::map<std::string, std::string>::iterator textureFilename = assetList.begin(); textureFilename != assetList.end(); textureFilename++) {
        GameTexture* texture = new GameTexture();

        this->logger->debug() << "Loading texture \"" << textureFilename->first << "\" from \"" << textureFilename->second << "\".";

        if (!texture->Load(textureFilename->second)) {
            return false;
        }

        this->textures[textureFilename->first] = texture;
    }

    return true;
}

//bool GameState::LoadFonts(std::string resourceListPath) {
//    this->logger->debug() << "Loading fonts from \"" << resourceListPath << "\"";
//
//    std::string jsonString;
//
//    if (!FileSystemHelpers::ReadFile(resourceListPath, jsonString)) {
//        this->logger->error() << "Failed to load resource list \"" << resourceListPath << "\".";
//
//        return false;
//    }
//    
//    AssetListParser parser = AssetListParser();
//    std::map<std::string, std::string> assetList;
//    parser.Parse(jsonString, &assetList);
//    this->logger->debug() << "parsed asset list \"" << resourceListPath << "\".";
//    
//    this->logger->debug() << "parsed asset list contents = {";
//    for (std::map<std::string, std::string>::iterator i = assetList.begin(); i != assetList.end(); i++) {
//        this->logger->debug() << (*i).first << " : " << (*i).second;
//    }
//    this->logger->debug() << "}";
//
//    for (std::map<std::string, std::string>::iterator fontFilename = assetList.begin(); fontFilename != assetList.end(); fontFilename++) {
//        GameFont* font = new GameFont();
//
//        // Bah.
//        int fontSize;
//
//        try {
//            fontSize = this->fontSizes.at(fontFilename->first);
//        } catch (const std::out_of_range& exception) {
//            fontSize = 10;
//        }
//
//        if (!font->Load(fontFilename->second, fontSize)) {
//            this->logger->error() << "failed to load " << fontFilename->second;
//
//            return false;
//        }
//
//        this->fonts[fontFilename->first] = font;
//    }
//    
//    return true;
//}

bool GameState::LoadSounds(std::string resourceListPath) {
    return true;
}

bool GameState::LoadSongs(std::string resourceListPath) {
    return true;
}
