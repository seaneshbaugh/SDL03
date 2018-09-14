#ifndef __SDL03__FontService__
#define __SDL03__FontService__

#include <memory>

#include "GameFont.h"

namespace Services {
    namespace Interfaces {
        class FontService {
        public:
            virtual ~FontService() {}
            virtual std::shared_ptr<GameFont> GetFont(const std::string fontName, const int fontSize) = 0;
        };
    }
}

#endif
