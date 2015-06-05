#include "GameState.h"

SDL_Renderer* GameState::renderer;

GameInputMapper* GameState::inputMapper;

GameParty* GameState::party = nullptr;

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
bool GameState::ReadFile(std::string filename, std::string &contents) {
    std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);

    try {
        if (in) {
            in.seekg(0, std::ios::end);

            contents.resize(in.tellg());

            in.seekg(0, std::ios::beg);

            in.read(&contents[0], contents.size());

            in.close();

            return true;
        } else {
            return false;
        }
    } catch (int exception) {
        return false;
    }
}

bool GameState::ParseResourceList(std::string jsonString, std::map<std::string, std::string> &resourceList) {
    JSONNode json = libjson::parse(jsonString);

    std::function<void(const JSONNode&)> parseJSON = [&] (const JSONNode &node) {
        JSONNode::const_iterator i = node.begin();

        while (i != node.end()) {
            if (i->type() == JSON_ARRAY || i->type() == JSON_NODE) {
                parseJSON(*i);
            }

            if (i->type() == JSON_STRING) {
                resourceList[i->name()] = i->as_string();
            }

            i++;
        }
    };
    
    parseJSON(json);
    
    return true;
}

bool GameState::LoadResources(std::string textureListPath, std::string fontListPath, std::string SoundListPath) {
    return this->LoadTextures(textureListPath) && this->LoadFonts(fontListPath) && this->LoadSounds(SoundListPath);
}

bool GameState::LoadTextures(std::string resourceListPath) {
    std::string jsonString;

    if (!this->ReadFile(resourceListPath, jsonString)) {
        return false;
    }

    std::map<std::string, std::string> textureList;

    if (!this->ParseResourceList(jsonString, textureList)) {
        return false;
    }

    for (std::map<std::string, std::string>::iterator textureFilename = textureList.begin(); textureFilename != textureList.end(); textureFilename++) {
        GameTexture* texture = new GameTexture();

        if (!texture->Load(textureFilename->second)) {
            return false;
        }

        this->textures[textureFilename->first] = texture;
    }

    return true;
}

bool GameState::LoadFonts(std::string resourceListPath) {
    std::string jsonString;

    if (!this->ReadFile(resourceListPath, jsonString)) {
        return false;
    }

    std::map<std::string, std::string> fontList;

    if (!this->ParseResourceList(jsonString, fontList)) {
        return false;
    }

    for (std::map<std::string, std::string>::iterator fontFilename = fontList.begin(); fontFilename != fontList.end(); fontFilename++) {
        GameFont* font = new GameFont();

        if (!font->Load(fontFilename->second)) {
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
