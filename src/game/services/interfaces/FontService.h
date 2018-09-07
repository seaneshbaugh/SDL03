#ifndef __SDL03__Fonts__
#define __SDL03__Fonts__

#include <memory>

#include "GameFont.h"

namespace Services {
    namespace Interfaces {
        class FontService {
        public:
            virtual ~FontService() {}
            virtual std::shared_ptr<GameFont> GetFont(std::string fontName) = 0;
            // virtual GameFont* GetFont(std::string fontName) = 0;
        };
    }
}

#endif
