#ifndef SDL03_Game_Services_Interfaces_FontService
#define SDL03_Game_Services_Interfaces_FontService

#include <memory>

namespace Game {
    namespace Resources {
        class Font;
    }

    namespace Services {
        namespace Interfaces {
            class FontService {
            public:
                virtual ~FontService() {}
                virtual std::shared_ptr<Resources::Font> GetFont(const std::string& fontName, const int fontSize) = 0;
            };
        }
    }
}

#endif
