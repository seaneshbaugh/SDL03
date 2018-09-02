#include "GameState.h"
#include "MapState.h"

// TODO: Move renderer and inputMapper to GameEngine?
// Now that I think about it they really shouldn't be tied to the states.
SDL_Renderer* GameState::renderer = nullptr;

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
}

GameState::~GameState() {
    for (std::map <std::string, GameTexture*>::iterator it = this->textures.begin(); it != this->textures.end(); it++) {
        if (it->second) {
            delete it->second;
        }
    }

    for (std::map <std::string, GameFont*>::iterator it = this->fonts.begin(); it != this->fonts.end(); it++) {
        if (it->second) {
            delete it->second;
        }
    }

    for (std::map <std::string, GameSound*>::iterator it = this->sounds.begin(); it != this->sounds.end(); it++) {
        if (it->second) {
            delete it->second;
        }
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
    if (event) {
        if (event->type == SDL_KEYDOWN) {
            this->ProcessInput(event->key.keysym.sym);
        }
    }

    return this;
}

std::string GameState::ProcessInput(int key) {
    return "";
}

void GameState::Render() {
    
}

// Now that I think about it some more maybe these belong on some sort of special
// container object that handles finding and instantiating the resources and also
// controls their access. Each state could have its own set of containers as
// needed. It might introduce more overhead, but it should make things cleaner.
//bool GameState::ReadFile(std::string filename, std::string &contents) {
//    std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);
//
//    try {
//        if (in) {
//            in.seekg(0, std::ios::end);
//
//            contents.resize(in.tellg());
//
//            in.seekg(0, std::ios::beg);
//
//            in.read(&contents[0], contents.size());
//
//            in.close();
//
//            return true;
//        } else {
//            return false;
//        }
//    } catch (int exception) {
//        return false;
//    }
//}

//bool GameState::ParseResourceList(std::string jsonString, std::map<std::string, std::string> &resourceList) {
//    JSONNode json = libjson::parse(jsonString);
//
//    std::function<void(const JSONNode&, std::string)> parseJSON = [&] (const JSONNode &node, std::string parentName) {
//        JSONNode::const_iterator i = node.begin();
//        
//        while (i != node.end()) {
//            std::string name;
//            if (parentName == "") {
//                name = i->name();
//            } else {
//                name = parentName + "." + i->name();
//            }
//            
//            std::cout << "name = " << name << std::endl;
//            
//            if (i->type() == JSON_ARRAY || i->type() == JSON_NODE) {
//                parseJSON(*i, name);
//            }
//
//            if (i->type() == JSON_STRING) {
//                std::cout << name << " => " << i->as_string() << std::endl;
//                resourceList[name] = i->as_string();
//            }
//
//            i++;
//        }
//    };
//    
//    parseJSON(json, "");
//    
//    return true;
//}

bool GameState::LoadResources(std::string textureListPath, std::string fontListPath, std::string SoundListPath) {
    std::cout << "Loading resource lists: \"" << textureListPath << "\", \"" << fontListPath << "\", \"" << SoundListPath << "\"." << std::endl;
    return this->LoadTextures(textureListPath) && this->LoadFonts(fontListPath) && this->LoadSounds(SoundListPath);
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
    
//    std::map<std::string, std::string> textureList;
//
//    if (!this->ParseResourceList(jsonString, textureList)) {
//        this->logger->error() << "Failed to parse resource list \"" << resourceListPath << "\".";
//
//        return false;
//    }

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

bool GameState::LoadFonts(std::string resourceListPath) {
    this->logger->debug() << "Loading fonts from \"" << resourceListPath << "\"";

    std::string jsonString;

    if (!FileSystemHelpers::ReadFile(resourceListPath, jsonString)) {
        this->logger->error() << "Failed to load resource list \"" << resourceListPath << "\".";

        return false;
    }
    
    AssetListParser parser = AssetListParser();
    std::map<std::string, std::string> assetList;
    parser.Parse(jsonString, &assetList);
    this->logger->debug() << "parsed asset list \"" << resourceListPath << "\".";
    
    this->logger->debug() << "parsed asset list contents = {";
    for (std::map<std::string, std::string>::iterator i = assetList.begin(); i != assetList.end(); i++) {
        this->logger->debug() << (*i).first << " : " << (*i).second;
    }
    this->logger->debug() << "}";

//    std::map<std::string, std::string> fontList;
//
//    if (!this->ParseResourceList(jsonString, fontList)) {
//        return false;
//    }

    for (std::map<std::string, std::string>::iterator fontFilename = assetList.begin(); fontFilename != assetList.end(); fontFilename++) {
        GameFont* font = new GameFont();

        // Bah.
        int fontSize;

        try {
            fontSize = this->fontSizes.at(fontFilename->first);
        } catch (const std::out_of_range& exception) {
            fontSize = 10;
        }

        if (!font->Load(fontFilename->second, fontSize)) {
            this->logger->error() << "failed to load " << fontFilename->second;

            return false;
        }

        this->fonts[fontFilename->first] = font;
    }
    
    return true;
}

bool GameState::LoadSounds(std::string resourceListPath) {
    return true;
}

bool GameState::LoadSongs(std::string resourceListPath) {
    return true;
}
