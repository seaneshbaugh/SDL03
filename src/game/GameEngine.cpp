#include "GameEngine.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int FRAMES_PER_SECOND = 60;
const std::string APPLICATION_NAME = "SDL03";

SDL_Renderer* GameEngine::currentRenderer = nullptr;

GameEngine::GameEngine() {
    this->screen = nullptr;

    this->renderer = nullptr;
    
    this->screenWidth = SCREEN_WIDTH;
    
    this->screenHeight = SCREEN_HEIGHT;

    this->windowTitle = APPLICATION_NAME;

    this->settings = new GameSettings(APPLICATION_NAME);

    this->logger = nullptr;
}

GameEngine::~GameEngine() {
    this->DestroyStates();

    if (this->screen) {
        SDL_DestroyWindow(this->screen);
    }

    Mix_CloseAudio();

    TTF_Quit();

    IMG_Quit();

    SDL_Quit();

    delete this->settings;
}

bool GameEngine::Setup() {
    if (!this->SetupLogging()) {
        return false;
    }

    if (!this->SetupSDL()) {
        return false;
    }



    if (!this->settings->LoadSettings()) {
        this->logger->critic() << "Failed to load settings.";

        return false;
    }

    this->logger->info() << "Settings loaded.";

    this->inputMapper.MapKeys(this->settings->InputSettings());

    GameState::inputMapper = &this->inputMapper;

    return true;
}

bool GameEngine::SetupLogging() {
#ifndef NDEBUG
    Log::Manager::setDefaultLevel(Log::Log::eDebug);
#else
    Log::Manager::setDefaultLevel(Log::Log::eError);
#endif

    const std::string logDirectory = FileSystemHelpers::GetLogDirectory();
    Log::Config::Vector configList;

    Log::Config::addOutput(configList, "OutputConsole");
    Log::Config::addOutput(configList, "OutputFile");
    Log::Config::setOption(configList, "filename", (logDirectory + "/SDL03.log").c_str());
    Log::Config::setOption(configList, "filename_old", (logDirectory + "/SDL03.old.log").c_str());
    Log::Config::setOption(configList, "max_startup_size", "0");
    Log::Config::setOption(configList, "max_size", "16777216");

    Log::Manager::configure(configList);

    // TODO: maybe add the ability to set this via a config file?
    Log::Manager::get("main")->setLevel(Log::Log::eDebug);
    Log::Manager::get("settings")->setLevel(Log::Log::eDebug);
    Log::Manager::get("json")->setLevel(Log::Log::eDebug);
    Log::Manager::get("lua")->setLevel(Log::Log::eInfo);
    Log::Manager::get("map")->setLevel(Log::Log::eInfo);
    Log::Manager::get("states.intro")->setLevel(Log::Log::eDebug);
    Log::Manager::get("states.main_menu")->setLevel(Log::Log::eDebug);
    Log::Manager::get("states.map")->setLevel(Log::Log::eInfo);
    Log::Manager::get("states.battle")->setLevel(Log::Log::eDebug);
    Log::Manager::get("assets.fonts")->setLevel(Log::Log::eDebug);
    Log::Manager::get("assets.songs")->setLevel(Log::Log::eDebug);
    Log::Manager::get("assets.sounds")->setLevel(Log::Log::eDebug);
    Log::Manager::get("assets.texures")->setLevel(Log::Log::eDebug);

    this->logger = new Log::Logger("main");

    logger->info() << "Logger initialized.";

    return true;
}

bool GameEngine::SetupSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        this->logger->critic() << "Failed to initialize SDL. " << SDL_GetError();

        return false;
    }

    this->logger->info() << "SDL initialized.";

    if (SDL_CreateWindowAndRenderer(this->screenWidth, this->screenHeight, SDL_WINDOW_OPENGL, &this->screen, &this->renderer)) {
        this->logger->critic() << "Failed to create window and renderer. " << SDL_GetError();

        return false;
    }

    this->logger->info() << "Window and render created.";

    SDL_SetWindowTitle(this->screen, this->windowTitle.c_str());

    GameEngine::currentRenderer = this->renderer;

    if ((IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG) != IMG_INIT_PNG) {
        this->logger->critic() << "Failed to initialize SDL_image. " << IMG_GetError();

        return false;
    }

    this->logger->info() << "SDL_image initialized.";

    if (TTF_Init() == -1) {
        this->logger->critic() << "Failed to initialize SDL_ttf. " << TTF_GetError();

        return false;
    }

    this->logger->info() << "SDL_ttf initialized.";

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        this->logger->critic() << "Failed to initialize SDL_mixer. " << Mix_GetError();

        return false;
    }

    this->logger->info() << "SDL_mixer initialized.";

    return true;
}

bool GameEngine::LoadFonts(std::string resourceListPath) {
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

    for (std::map<std::string, std::string>::iterator fontFilename = assetList.begin(); fontFilename != assetList.end(); fontFilename++) {
        GameFont* font = new GameFont();

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

void GameEngine::Start() {
    IntroState* introState = new IntroState(nullptr);

    this->states.push_back(introState);

    this->MainLoop();
}

void GameEngine::MainLoop() {
    SDL_Event event;

    bool quit = false;

    unsigned long long frameCount = 0;

    // The way this works right now is very tentative and is probably very bad. I have
    // no idea what I'm doing, but I think this should work while things are still
    // simple.
    // The basic idea is, each pass of the loop checks to see if there is a state
    // object left in the stack (I'm using a std::vector like a stack because I'm
    // lazy), if not then do nothing and then quit. Otherwise, check for any pending
    // events then call the update function for the current state (and quit if
    // desired).
    // This is the tricky part that I'm not convinced is a good idea: if the Update
    // function returns nullptr then we pop the current state off the end of the stack.
    // If it returns itself we do nothing. And finally, if it returns a pointer to
    // another GameState object then that state is pushed onto the stack and is now the
    // current state.
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
            int startTicks = SDL_GetTicks();

            GameState* currentState = this->states.back();

            GameState* nextState = currentState;

            int pendingEvent = SDL_PollEvent(&event);

            int key = NO_KEY;

            if (pendingEvent) {
                if (event.type == SDL_QUIT) {
                    break;
                }

                key = this->inputMapper.GetInputMapKey(&event);
            }

            if (currentState->acceptRawInput) {
                nextState = currentState->Update(&event);
            } else {
                nextState = currentState->Update(key);
            }

            this->Render();

            if (nextState == nullptr) {
                delete currentState;

                this->states.pop_back();
            } else {
                if (nextState != currentState) {
                    this->states.push_back(nextState);
                }
            }

            if ((SDL_GetTicks() - startTicks) < (1000 / FRAMES_PER_SECOND)) {
                SDL_Delay((1000 / FRAMES_PER_SECOND) - (SDL_GetTicks() - startTicks));
            }
        } else {
            quit = true;
        }

        frameCount++;
    }
}

void GameEngine::Render() {
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);

    SDL_RenderClear(this->renderer);

    this->states.back()->Render();

    SDL_RenderPresent(this->renderer);
}

void GameEngine::DestroyStates() {
    for (std::vector <GameState*>::iterator state = this->states.begin(); state != this->states.end(); state++) {
        if (*state != nullptr) {
            delete *state;
        }
    }
}
