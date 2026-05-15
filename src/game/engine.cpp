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
        if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD) == false) {
            this->logger->critic() << "Failed to initialize SDL. " << SDL_GetError();
        }

        this->logger->info() << "SDL initialized.";
    }

    void Engine::StartVideoService() {
        // TODO: Don't hardcode viewport dimensions here.
        // First we should check the settings to see if they contain viewport dimensions and whether or not the game should be in fullscreen mode.
        // If the settings don't contain viewport dimensions then we can use the current display mode's dimensions as the default.
        // We can also add some settings options for scaling the viewport up or down from the display mode's dimensions.
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
        bool quit = false;

        //SDL_Gamepad* gamepad = nullptr;

        // Each pass of the main game loop checks to see if there is a state object
        // left in the stack. If there isn't then the loop stops and the game quits. If
        // there is a state object in the stack then the loop polls for events, passing
        // each event to the current state to handle, until there are no more events. If
        // a quit event is received then the loop stops and the game quits. Otherwise,
        // the current state's Update function is called with the amount of time that
        // has elapsed since the last frame. The Update function returns a
        // States::Transition struct that contains a type (None, Pop, Push, Replace,
        // etc.) and a pointer to the next state if applicable. If the type of the
        // States::Transition struct is Pop then we pop the current state off the stack.
        // If the type is Push then we push the next state on the stack. If the type is
        // Replace then we pop the current state off the stack and then push the next
        // state on the stack. If the type is None then we do nothing to the stack. This
        // ensures that only states can initiate the transition to a new state and that
        // main loop itself remains completely unaware of what each state actually is
        // and how transitions between states work. Each state can manage its own
        // internal objects and decide when it wants to transition to a new state and
        // what that new state should be without needing to know anything about the
        // Engine's state stack or how the loop works.
        while (!this->states.empty()) {
            int startTicks = SDL_GetTicks();
            Services::Locator::TimeService()->BeginFrame();

            std::shared_ptr<States::Base> currentState = this->states.top();

            while (SDL_PollEvent(&event)) {
                this->logger->debug() << "Received event: " << EventTypeName(event);

                if (event.type == SDL_EVENT_QUIT) {
                    quit = true;

                    break;
                } else {
                    //// This is a very dirty way of handling this. I just want to see a gamepad
                    //// working. Eventually this should be moved to the InputManager.
                    //switch (event.type) {
                    //case SDL_EVENT_GAMEPAD_ADDED:
                    //    SDL_JoystickID id = event.gdevice.which;

                    //    if (!gamepad) {
                    //        gamepad = SDL_OpenGamepad(id);
                    //    }
                    //}

                    currentState->HandleEvent(event);
                }
            }

            if (quit) {
                break;
            }

            States::Transition transition = currentState->Update(Services::Locator::TimeService()->GetDeltaTime());

            this->Render();

            switch (transition.type) {
            case States::Transition::Type::Push:
                this->states.push(transition.nextState);

                break;
            case States::Transition::Type::Pop:
                this->states.pop();

                break;
            case States::Transition::Type::Replace:
                this->states.pop();
                this->states.push(transition.nextState);

                break;
            case States::Transition::Type::Quit:
                while (!this->states.empty()) {
                    this->states.pop();
                }
            }

            if ((SDL_GetTicks() - startTicks) < (1000 / FRAMES_PER_SECOND)) {
                SDL_Delay((1000 / FRAMES_PER_SECOND) - (SDL_GetTicks() - startTicks));
            }

            Services::Locator::TimeService()->IncrementFrameCount();
        }

        //if (gamepad) {
        //    SDL_CloseGamepad(gamepad);
        //}
    }

    // TODO: Strongly consider making this a method on the VideoService. The current state's
    // Render method could maybe be passed as a callback.

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

    std::string Engine::EventTypeName(const SDL_Event& event) {
        switch (event.type) {
        case SDL_EVENT_FIRST:
            return "SDL_EVENT_FIRST";

        // App events
        case SDL_EVENT_QUIT:
            return "SDL_EVENT_QUIT";
        case SDL_EVENT_TERMINATING:
            return "SDL_EVENT_TERMINATING";
        case SDL_EVENT_LOW_MEMORY:
            return "SDL_EVENT_LOW_MEMORY";
        case SDL_EVENT_WILL_ENTER_BACKGROUND:
            return "SDL_EVENT_WILL_ENTER_BACKGROUND";
        case SDL_EVENT_DID_ENTER_BACKGROUND:
            return "SDL_EVENT_DID_ENTER_BACKGROUND";
        case SDL_EVENT_WILL_ENTER_FOREGROUND:
            return "SDL_EVENT_WILL_ENTER_FOREGROUND";
        case SDL_EVENT_DID_ENTER_FOREGROUND:
            return "SDL_EVENT_DID_ENTER_FOREGROUND";
        case SDL_EVENT_LOCALE_CHANGED:
            return "SDL_EVENT_LOCALE_CHANGED";
        case SDL_EVENT_SYSTEM_THEME_CHANGED:
            return "SDL_EVENT_SYSTEM_THEME_CHANGED";

        // Display events
        case SDL_EVENT_DISPLAY_ORIENTATION:
            return "SDL_EVENT_DISPLAY_ORIENTATION";
        case SDL_EVENT_DISPLAY_ADDED:
            return "SDL_EVENT_DISPLAY_ADDED";
        case SDL_EVENT_DISPLAY_REMOVED:
            return "SDL_EVENT_DISPLAY_REMOVED";
        case SDL_EVENT_DISPLAY_MOVED:
            return "SDL_EVENT_DISPLAY_MOVED";
        case SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED:
            return "SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED";
        case SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED:
            return "SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED";
        case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED:
            return "SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED";
        case SDL_EVENT_DISPLAY_USABLE_BOUNDS_CHANGED:
            return "SDL_EVENT_DISPLAY_USABLE_BOUNDS_CHANGED";

        // Window events
        case SDL_EVENT_WINDOW_SHOWN:
            return "SDL_EVENT_WINDOW_SHOWN";
        case SDL_EVENT_WINDOW_HIDDEN:
            return "SDL_EVENT_WINDOW_HIDDEN";
        case SDL_EVENT_WINDOW_EXPOSED:
            return "SDL_EVENT_WINDOW_EXPOSED";
        case SDL_EVENT_WINDOW_MOVED:
            return "SDL_EVENT_WINDOW_MOVED";
        case SDL_EVENT_WINDOW_RESIZED:
            return "SDL_EVENT_WINDOW_RESIZED";
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
            return "SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED";
        case SDL_EVENT_WINDOW_METAL_VIEW_RESIZED:
            return "SDL_EVENT_WINDOW_METAL_VIEW_RESIZED";
        case SDL_EVENT_WINDOW_MINIMIZED:
            return "SDL_EVENT_WINDOW_MINIMIZED";
        case SDL_EVENT_WINDOW_MAXIMIZED:
            return "SDL_EVENT_WINDOW_MAXIMIZED";
        case SDL_EVENT_WINDOW_RESTORED:
            return "SDL_EVENT_WINDOW_RESTORED";
        case SDL_EVENT_WINDOW_MOUSE_ENTER:
            return "SDL_EVENT_WINDOW_MOUSE_ENTER";
        case SDL_EVENT_WINDOW_MOUSE_LEAVE:
            return "SDL_EVENT_WINDOW_MOUSE_LEAVE";
        case SDL_EVENT_WINDOW_FOCUS_GAINED:
            return "SDL_EVENT_WINDOW_FOCUS_GAINED";
        case SDL_EVENT_WINDOW_FOCUS_LOST:
            return "SDL_EVENT_WINDOW_FOCUS_LOST";
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            return "SDL_EVENT_WINDOW_CLOSE_REQUESTED";
        case SDL_EVENT_WINDOW_HIT_TEST:
            return "SDL_EVENT_WINDOW_HIT_TEST";
        case SDL_EVENT_WINDOW_ICCPROF_CHANGED:
            return "SDL_EVENT_WINDOW_ICCPROF_CHANGED";
        case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
            return "SDL_EVENT_WINDOW_DISPLAY_CHANGED";
        case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
            return "SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED";
        case SDL_EVENT_WINDOW_SAFE_AREA_CHANGED:
            return "SDL_EVENT_WINDOW_SAFE_AREA_CHANGED";
        case SDL_EVENT_WINDOW_OCCLUDED:
            return "SDL_EVENT_WINDOW_OCCLUDED";
        case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
            return "SDL_EVENT_WINDOW_ENTER_FULLSCREEN";
        case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
            return "SDL_EVENT_WINDOW_LEAVE_FULLSCREEN";
        case SDL_EVENT_WINDOW_DESTROYED:
            return "SDL_EVENT_WINDOW_DESTROYED";
        case SDL_EVENT_WINDOW_HDR_STATE_CHANGED:
            return "SDL_EVENT_WINDOW_HDR_STATE_CHANGED";

        // Keyboard events
        case SDL_EVENT_KEY_DOWN:
            return "SDL_EVENT_KEY_DOWN";
        case SDL_EVENT_KEY_UP:
            return "SDL_EVENT_KEY_UP";
        case SDL_EVENT_TEXT_EDITING:
            return "SDL_EVENT_TEXT_EDITING";
        case SDL_EVENT_TEXT_INPUT:
            return "SDL_EVENT_TEXT_INPUT";
        case SDL_EVENT_KEYMAP_CHANGED:
            return "SDL_EVENT_KEYMAP_CHANGED";
        case SDL_EVENT_KEYBOARD_ADDED:
            return "SDL_EVENT_KEYBOARD_ADDED";
        case SDL_EVENT_KEYBOARD_REMOVED:
            return "SDL_EVENT_KEYBOARD_REMOVED";
        case SDL_EVENT_TEXT_EDITING_CANDIDATES:
            return "SDL_EVENT_TEXT_EDITING_CANDIDATES";
        case SDL_EVENT_SCREEN_KEYBOARD_SHOWN:
            return "SDL_EVENT_SCREEN_KEYBOARD_SHOWN";
        case SDL_EVENT_SCREEN_KEYBOARD_HIDDEN:
            return "SDL_EVENT_SCREEN_KEYBOARD_HIDDEN";

        // Mouse events
        case SDL_EVENT_MOUSE_MOTION:
            return "SDL_EVENT_MOUSE_MOTION";
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            return "SDL_EVENT_MOUSE_BUTTON_DOWN";
        case SDL_EVENT_MOUSE_BUTTON_UP:
            return "SDL_EVENT_MOUSE_BUTTON_UP";
        case SDL_EVENT_MOUSE_WHEEL:
            return "SDL_EVENT_MOUSE_WHEEL";
        case SDL_EVENT_MOUSE_ADDED:
            return "SDL_EVENT_MOUSE_ADDED";
        case SDL_EVENT_MOUSE_REMOVED:
            return "SDL_EVENT_MOUSE_REMOVED";

        // Joystick events
        case SDL_EVENT_JOYSTICK_AXIS_MOTION:
            return "SDL_EVENT_JOYSTICK_AXIS_MOTION";
        case SDL_EVENT_JOYSTICK_BALL_MOTION:
            return "SDL_EVENT_JOYSTICK_BALL_MOTION";
        case SDL_EVENT_JOYSTICK_HAT_MOTION:
            return "SDL_EVENT_JOYSTICK_HAT_MOTION";
        case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
            return "SDL_EVENT_JOYSTICK_BUTTON_DOWN";
        case SDL_EVENT_JOYSTICK_BUTTON_UP:
            return "SDL_EVENT_JOYSTICK_BUTTON_UP";
        case SDL_EVENT_JOYSTICK_ADDED:
            return "SDL_EVENT_JOYSTICK_ADDED";
        case SDL_EVENT_JOYSTICK_REMOVED:
            return "SDL_EVENT_JOYSTICK_REMOVED";
        case SDL_EVENT_JOYSTICK_BATTERY_UPDATED:
            return "SDL_EVENT_JOYSTICK_BATTERY_UPDATED";
        case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE:
            return "SDL_EVENT_JOYSTICK_UPDATE_COMPLETE";

        // Gamepad events
        case SDL_EVENT_GAMEPAD_AXIS_MOTION:
            return "SDL_EVENT_GAMEPAD_AXIS_MOTION";
        case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            return "SDL_EVENT_GAMEPAD_BUTTON_DOWN";
        case SDL_EVENT_GAMEPAD_BUTTON_UP:
            return "SDL_EVENT_GAMEPAD_BUTTON_UP";
        case SDL_EVENT_GAMEPAD_ADDED:
            return "SDL_EVENT_GAMEPAD_ADDED";
        case SDL_EVENT_GAMEPAD_REMOVED:
            return "SDL_EVENT_GAMEPAD_REMOVED";
        case SDL_EVENT_GAMEPAD_REMAPPED:
            return "SDL_EVENT_GAMEPAD_REMAPPED";
        case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN:
            return "SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN";
        case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION:
            return "SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION";
        case SDL_EVENT_GAMEPAD_TOUCHPAD_UP:
            return "SDL_EVENT_GAMEPAD_TOUCHPAD_UP";
        case SDL_EVENT_GAMEPAD_SENSOR_UPDATE:
            return "SDL_EVENT_GAMEPAD_SENSOR_UPDATE";
        case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE:
            return "SDL_EVENT_GAMEPAD_UPDATE_COMPLETE";
        case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED:
            return "SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED";

        // Touch events
        case SDL_EVENT_FINGER_DOWN:
            return "SDL_EVENT_FINGER_DOWN";
        case SDL_EVENT_FINGER_UP:
            return "SDL_EVENT_FINGER_UP";
        case SDL_EVENT_FINGER_MOTION:
            return "SDL_EVENT_FINGER_MOTION";
        case SDL_EVENT_FINGER_CANCELED:
            return "SDL_EVENT_FINGER_CANCELED";

        // Pinch events
        case SDL_EVENT_PINCH_BEGIN:
            return "SDL_EVENT_PINCH_BEGIN";
        case SDL_EVENT_PINCH_UPDATE:
            return "SDL_EVENT_PINCH_UPDATE";
        case SDL_EVENT_PINCH_END:
            return "SDL_EVENT_PINCH_END";

        // Clipboard events
        case SDL_EVENT_CLIPBOARD_UPDATE:
            return "SDL_EVENT_CLIPBOARD_UPDATE";

        // Drag and drop events
        case SDL_EVENT_DROP_FILE:
            return "SDL_EVENT_DROP_FILE";
        case SDL_EVENT_DROP_TEXT:
            return "SDL_EVENT_DROP_TEXT";
        case SDL_EVENT_DROP_BEGIN:
            return "SDL_EVENT_DROP_BEGIN";
        case SDL_EVENT_DROP_COMPLETE:
            return "SDL_EVENT_DROP_COMPLETE";
        case SDL_EVENT_DROP_POSITION:
            return "SDL_EVENT_DROP_POSITION";

        // Audio events
        case SDL_EVENT_AUDIO_DEVICE_ADDED:
            return "SDL_EVENT_AUDIO_DEVICE_ADDED";
        case SDL_EVENT_AUDIO_DEVICE_REMOVED:
            return "SDL_EVENT_AUDIO_DEVICE_REMOVED";
        case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED:
            return "SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED";

        // Sensor events
        case SDL_EVENT_SENSOR_UPDATE:
            return "SDL_EVENT_SENSOR_UPDATE";

        // Pen events
        case SDL_EVENT_PEN_PROXIMITY_IN:
            return "SDL_EVENT_PEN_PROXIMITY_IN";
        case SDL_EVENT_PEN_PROXIMITY_OUT:
            return "SDL_EVENT_PEN_PROXIMITY_OUT";
        case SDL_EVENT_PEN_DOWN:
            return "SDL_EVENT_PEN_DOWN";
        case SDL_EVENT_PEN_UP:
            return "SDL_EVENT_PEN_UP";
        case SDL_EVENT_PEN_BUTTON_DOWN:
            return "SDL_EVENT_PEN_BUTTON_DOWN";
        case SDL_EVENT_PEN_BUTTON_UP:
            return "SDL_EVENT_PEN_BUTTON_UP";
        case SDL_EVENT_PEN_MOTION:
            return "SDL_EVENT_PEN_MOTION";
        case SDL_EVENT_PEN_AXIS:
            return "SDL_EVENT_PEN_AXIS";

        // Camera events
        case SDL_EVENT_CAMERA_DEVICE_ADDED:
            return "SDL_EVENT_CAMERA_DEVICE_ADDED";
        case SDL_EVENT_CAMERA_DEVICE_REMOVED:
            return "SDL_EVENT_CAMERA_DEVICE_REMOVED";
        case SDL_EVENT_CAMERA_DEVICE_APPROVED:
            return "SDL_EVENT_CAMERA_DEVICE_APPROVED";
        case SDL_EVENT_CAMERA_DEVICE_DENIED:
            return "SDL_EVENT_CAMERA_DEVICE_DENIED";

        // Render events
        case SDL_EVENT_RENDER_TARGETS_RESET:
            return "SDL_EVENT_RENDER_TARGETS_RESET";
        case SDL_EVENT_RENDER_DEVICE_RESET:
            return "SDL_EVENT_RENDER_DEVICE_RESET";
        case SDL_EVENT_RENDER_DEVICE_LOST:
            return "SDL_EVENT_RENDER_DEVICE_LOST";

        // Private events
        case SDL_EVENT_PRIVATE0:
            return "SDL_EVENT_PRIVATE0";
        case SDL_EVENT_PRIVATE1:
            return "SDL_EVENT_PRIVATE1";
        case SDL_EVENT_PRIVATE2:
            return "SDL_EVENT_PRIVATE2";
        case SDL_EVENT_PRIVATE3:
            return "SDL_EVENT_PRIVATE3";

        // Internal events
        case SDL_EVENT_POLL_SENTINEL:
            return "SDL_EVENT_POLL_SENTINEL";

        // User events
        case SDL_EVENT_USER:
            return "SDL_EVENT_USER";

        case SDL_EVENT_LAST:
            return "SDL_EVENT_LAST";

        default:
            return "UNKNOWN";
        }
    }
}
