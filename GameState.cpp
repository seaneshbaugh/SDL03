#include "GameState.h"

GameState::GameState(SDL_Renderer* renderer) {
    this->renderer = renderer;

    this->pop = false;
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

GameState* GameState::Update(SDL_Event* event) {
    if (event) {
        this->ProcessInput(event);
    }

    if (this->pop) {
        return NULL;
    } else {
        return this;
    }
}

void GameState::ProcessInput(SDL_Event* event) {

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

bool GameState::LoadResources() {
    return this->LoadTextures() && this->LoadFonts() && this->LoadSounds();
}

bool GameState::LoadTextures() {
    return true;
}

bool GameState::LoadFonts() {
    return true;
}

bool GameState::LoadSounds() {
    return true;
}