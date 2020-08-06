#include "engine.hpp"

namespace Game {
    const std::string Engine::logChannel = "engine";

    Engine::Engine() {
        this->StartSystem();
        this->StartLoggerService();
        this->StartTimeService();
        this->StartSettingsService();
        this->StartSDL();
        this->StartVideoService();
        this->StartTextureService();
        this->StartFontService();
        this->StartAudioService();
        this->StartInputService();
        this->StartItemService();
        this->StartWorldService();
        this->StartSaveService();
    }

    Engine::~Engine() {
        this->StopSaveService();
        this->StopWorldService();
        this->StopItemService();
        this->StopInputService();
        this->StopAudioService();
        this->StopFontService();
        this->StopTextureService();
        this->StopVideoService();
        this->StopSDL();
        this->StopSettingsService();
        this->StopTimeService();
        this->StopLoggerService();
        this->StopSystem();
    }

    int Engine::Start() {
        this->states.push(std::make_shared<States::Intro>());

        this->MainLoop();

        return 0;
    }

    // TODO: Put each OS's version of Engine::StartSystem in its own method.
    void Engine::StartSystem() {
#ifdef __APPLE__
        char resourceDirectory[MAXPATHLEN];

        if (CFURLGetFileSystemRepresentation(CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle()), 1, reinterpret_cast<UInt8*>(resourceDirectory), MAXPATHLEN)) {
            chdir(resourceDirectory);
        }
#elif _WIN32
        // For now nothing to do.
#elif __linux
#error Linux is not yet supported.
#else
#error Unknown and unsupported platform.
#endif
    }

    void Engine::StartLoggerService() {
        Services::Locator::ProvideService(std::make_shared<Services::Implementations::LoggerManager>());

        this->logger = Services::Locator::LoggerService()->GetLogger(Engine::logChannel);
    }

    void Engine::StartTimeService() {
        Services::Locator::ProvideService(std::make_shared<Services::Implementations::Clock>());
    }

    void Engine::StartSettingsService() {
        Services::Locator::ProvideService(std::make_shared<Services::Implementations::SettingsManager>(Game::APPLICATION_NAME));
    }

    void Engine::StartSDL() {
        if (SDL_Init(0) < 0) {
            this->logger->critic() << "Failed to initialize SDL. " << SDL_GetError();
        }

        this->logger->info() << "SDL initialized.";
    }

    void Engine::StartVideoService() {
        Services::Locator::ProvideService(std::make_shared<Services::Implementations::VideoManager>(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, Game::APPLICATION_NAME));
    }

    void Engine::StartTextureService() {
        Services::Locator::ProvideService(std::make_shared<Services::Implementations::TextureManager>());
    }

    void Engine::StartFontService() {
        Services::Locator::ProvideService(std::make_shared<Services::Implementations::FontManager>("resources/asset_lists/fonts.json"));
    }

    void Engine::StartAudioService() {
        Services::Locator::ProvideService(std::make_shared<Services::Implementations::AudioManager>());
    }

    void Engine::StartInputService() {
        Services::Locator::ProvideService(std::make_shared<Services::Implementations::InputManager>());
    }

    void Engine::StartItemService() {
        Services::Locator::ProvideService(std::make_shared<Services::Implementations::ItemManager>());
    }

    void Engine::StartWorldService() {
        Services::Locator::ProvideService(std::make_shared<Services::Implementations::WorldManager>());
    }

    void Engine::StartSaveService() {
        Services::Locator::ProvideService(std::make_shared<Services::Implementations::SaveManager>());
    }

    void Engine::MainLoop() {
        SDL_Event event;

        // The way this works right now is very tentative and is probably very bad. I have
        // no idea what I'm doing, but I think this should work while things are still
        // simple.
        // The basic idea is, each pass of the loop checks to see if there is a state
        // object left in the stack, if not then do nothing and then quit. Otherwise, check
        // for any pending events then call the update function for the current state (and
        // quit if desired).
        // This is the tricky part that I'm not convinced is a good idea: if the Update
        // function returns nullptr then we pop the current state off the end of the stack.
        // If it returns itself we do nothing. And finally, if it returns a pointer to
        // another GameState object then that state is pushed onto the stack and is now the
        // current state.
        // Without passing the whole Engine object to the update function I can't
        // really figure out a better way of telling the Engine to do stuff with its
        // state stack.
        // I'm hoping that doing things this way will ensure that only the current state
        // initiates transitions to new states and by having that transition be opaque to
        // the loop, the loop itself should remain very simple and unaware of what each
        // state actually means. It should also allow each state to manage its own internal
        // objects independently.
        while (this->states.size() > 0) {
            int startTicks = SDL_GetTicks();
            std::shared_ptr<States::Base> currentState = this->states.top();
            std::shared_ptr<States::Base> nextState = currentState;
            int pendingEvent = SDL_PollEvent(&event);
            // TODO: Get rid of this casting.
            int key = static_cast<int>(InputKey::NO_KEY);

            if (pendingEvent) {
                if (event.type == SDL_QUIT) {
                    break;
                }

                key = static_cast<int>(Services::Locator::InputService()->GetInputMapKey(event));
            }

            if (currentState->acceptRawInput) {
                nextState = currentState->Update(event);
            } else {
                nextState = currentState->Update(key);
            }

            this->Render();

            if (nextState == nullptr) {
                this->states.pop();
            } else {
                if (nextState != currentState) {
                    this->states.push(nextState);
                }
            }

            if ((SDL_GetTicks() - startTicks) < (1000 / FRAMES_PER_SECOND)) {
                SDL_Delay((1000 / FRAMES_PER_SECOND) - (SDL_GetTicks() - startTicks));
            }

            Services::Locator::TimeService()->IncrementFrameCount();
        }
    }

    // TODO: Strongly consider making this a method on the VideoService. The current Render
    // method should probably be renamed RenderSprite. The current state's Render method
    // could maybe be passed as a callback.

    // TODO: The VideoService Render function can take a list of visible states to render,
    // in order from bottom to top (so that the stuff in a higher state renders on top of
    // the stuff in a lower state). The main loop can look through the states (which might
    // mean making it be a std::vector again) starting with the current state, working
    // down until it finds one that is "opaque" which will be used as the bottom of the
    // rendering list. This will allow for, e.g., a Map state to be on the bottom and then
    // a Menu state to be on top of it without them needing to know anything about each
    // other.
    void Engine::Render() {
        Services::Locator::VideoService()->ClearScreen();

        this->states.top()->Render();

        Services::Locator::VideoService()->UpdateScreen();
    }

    // This is probably unecessary because the states are smart pointers in a std::stack.
    void Engine::DestroyStates() {
//        for (auto it = this->states.begin(); it != this->states.end(); ++it) {
//            if (*it != nullptr) {
//                (*it)->reset();
//            }
//        }
    }

    void Engine::StopSaveService() {
        Services::Locator::StopSaveService();
    }

    void Engine::StopWorldService() {
        Services::Locator::StopWorldService();
    }

    void Engine::StopItemService() {
        Services::Locator::StopItemService();
    }

    void Engine::StopInputService() {
        Services::Locator::StopInputService();
    }

    void Engine::StopAudioService() {
        Services::Locator::StopAudioService();
    }

    void Engine::StopFontService() {
        Services::Locator::StopFontService();
    }

    void Engine::StopTextureService() {
        Services::Locator::StopTextureService();
    }

    void Engine::StopVideoService() {
        Services::Locator::StopVideoService();
    }

    void Engine::StopSDL() {
        SDL_Quit();
    }

    void Engine::StopSettingsService() {
        Services::Locator::StopSettingsService();
    }

    void Engine::StopTimeService() {
        Services::Locator::StopTimeService();
    }

    void Engine::StopLoggerService() {
        Services::Locator::StopLoggerService();
    }

    void Engine::StopSystem() {
    }
}
