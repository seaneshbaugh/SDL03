#include "GameEngine.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

GameEngine::GameEngine() {
    this->screen = NULL;

    this->renderer = NULL;
    
    this->screenWidth = SCREEN_WIDTH;
    
    this->screenHeight = SCREEN_WIDTH;

    this->windowTitle = "Hello, world!";
}

GameEngine::~GameEngine() {
    for (std::vector<GameTexture*>::iterator texture = this->textures.begin(); texture != this->textures.end(); texture++) {
        if (*texture) {
            delete *texture;
        }
    }

    for (std::vector<GameFont*>::iterator font = this->fonts.begin(); font != this->fonts.end(); font++) {
        if (*font) {
            delete *font;
        }
    }

    for (std::vector<GameSound*>::iterator sound = this->sounds.begin(); sound != this->sounds.end(); sound++) {
        if (*sound) {
            delete *sound;
        }
    }

    if (this->screen) {
        SDL_DestroyWindow(this->screen);
    }

    SDL_Quit();
}

bool GameEngine::Setup() {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        return false;
    }

    SDL_CreateWindowAndRenderer(this->screenWidth, this->screenHeight, SDL_WINDOW_OPENGL, &this->screen, &this->renderer);

    if (this->screen == NULL || this->renderer == NULL) {
        return false;
    }

    if (TTF_Init() == -1) {
        return false;
    }

    if (this->LoadResources() == false) {
        return false;
    }

    return true;
}

bool GameEngine::LoadResources() {
    return this->LoadTextures() && this->LoadFonts() && this->LoadSounds();
}

// Eventually each resource will have a name or ID or something so they can be
// referenced without necessarily knowing the order in which they were loaded.
// These functions may be moved to the GameState class so that each state will keep
// track of its own resources. I'm not 100% sold on that idea though since it will
// likely mean lots of potential duplication of loaded objects. Whether or not that
// actually matters, I don't know.
bool GameEngine::LoadTextures() {
    return true;
}

bool GameEngine::LoadFonts() {
    std::string jsonString;

    if (!this->ReadFile("fonts.json", jsonString)) {
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

        this->fonts.push_back(font);
    }

    return true;
}

bool GameEngine::LoadSounds() {
    return true;
}

// This is only going here because C++ doesn't have class methods (in the Ruby
// sense) so I can't put it in with GameResource, which is where I would put it,
// but it's not really something that belongs on an instance of anything. I'm
// really afraid that GameEngine will end up as a junk drawer.
bool GameEngine::ReadFile(std::string filename, std::string &contents) {
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

bool GameEngine::ParseResourceList(std::string jsonString, std::map<std::string, std::string> &resourceList) {
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


void GameEngine::Start() {
    IntroState* introState = new IntroState(this->renderer);

    this->states.push_back(introState);

    this->MainLoop();
}

void GameEngine::MainLoop() {
    SDL_Event event;

    bool quit = false;

    // The way this works right now is very tentative and is probably very bad. I have
    // no idea what I'm doing, but I think this should work while things are still
    // simple.
    // The basic idea is, each pass of the loop checks to see if there is a state
    // object left in the stack (I'm using a std::vector like a stack because I'm
    // lazy), if not then do nothing and then quit. Otherwise, check for any pending
    // events then call the update function for the current state (and quit if
    // desired).
    // This is the tricky part that I'm not convinced is a good idea: if the Update
    // function returns NULL then we pop the current state off the end of the stack. If
    // it returns itself we do nothing. And finally, if it returns a pointer to another
    // GameState object then that state is pushed onto the stack and is now the current
    // state.
    // Without passing the whole GameEngine object to the update function I can't
    // really figure out a better way of telling the GameEngine to do stuff with its
    // state stack.
    // I'm hoping that doing things this way will ensure that only the current state
    // initiates transitions to new states and by having that transition be opaque to
    // the loop, the loop itself should remain very simple and unaware of what each
    // state actually means. It should also allow each state to manage its own internal
    // objects independently.
    while (!quit) {
        if (this->states.size() > 0) {
            int pendingEvent;

            pendingEvent = SDL_PollEvent(&event);

            GameState* currentState = this->states.back();

            GameState* nextState = currentState;

            if (pendingEvent) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                }

                nextState = currentState->Update(&event);
            } else {
                nextState = currentState->Update(NULL);
            }

            this->Render();

            if (nextState == NULL) {
                this->states.pop_back();
            } else {
                if (nextState != currentState) {
                    this->states.push_back(nextState);
                }
            }
        } else {
            quit = true;
        }
    }
}

void GameEngine::Render() {
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);

    SDL_RenderClear(this->renderer);

    this->states.back()->RenderObjects(this->textures, this->fonts, this->sounds);

    SDL_RenderPresent(this->renderer);

    SDL_Delay(1000 / 60);
}
