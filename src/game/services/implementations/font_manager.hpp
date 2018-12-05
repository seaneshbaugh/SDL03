#ifndef SDL03_Game_Services_Implementations_FontManager
#define SDL03_Game_Services_Implementations_FontManager

#include <map>
#include <utility>

#include "SDL.h"
#include "SDL_ttf.h"

#include "../../../helpers/file_system.hpp"
#include "../../parsers/asset_list_parser.hpp"
#include "../interfaces/font_service.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            class FontManager : public Interfaces::FontService {
            public:
                FontManager(const std::string& fontAssetListPath);
                ~FontManager();
                std::shared_ptr<Resources::Font> GetFont(const std::string& fontName, const int fontSize);
            private:
                static const std::string logChannel;

                std::shared_ptr<Log::Logger> logger;
                std::map<std::pair<std::string, int>, std::shared_ptr<Resources::Font>> fonts;

                bool LoadFonts(const std::string& fontAssetListPath);
            };
        }
    }
}

#endif
