#ifndef SDL03_Game_Services_Implementations_VideoManager
#define SDL03_Game_Services_Implementations_VideoManager

#include "../locator.hpp"
#include "../interfaces/video_service.hpp"

namespace Game {
    namespace Resources {
        class Texture;
    }

    namespace Services {
        namespace Implementations {
            class VideoManager : public Interfaces::VideoService {
            public:
                VideoManager(const int screenWidth, const int screenHeight, const std::string& windowTitle);
                ~VideoManager();
                void ClearScreen();
                bool Render(SDL_Texture* texture, const SDL_Rect* const srcrect, const SDL_Rect* const dstrect);
                bool Render(std::shared_ptr<Resources::Texture> texture, const SDL_Rect* const srcrect, const SDL_Rect* const dstrect);
                void UpdateScreen();
                std::shared_ptr<SDL_Renderer> GetRenderer();

            private:
                static const std::string logChannel;

                std::shared_ptr<Log::Logger> logger;
                std::shared_ptr<SDL_Window> window;
                std::shared_ptr<SDL_Renderer> renderer;
                int screenWidth;
                int screenHeight;
                std::string windowTitle;
            };
        }
    }
}

#endif
