#ifndef __SDL03__FontManager__
#define __SDL03__FontManager__

#include <map>
#include <utility>

#include "FileSystemHelpers.h"

#include "AssetListParser.h"
#include "../interfaces/FontService.h"

namespace Services {
    namespace Implementations {
        class FontManager : public Interfaces::FontService {
        public:
            FontManager(const std::string fontAssetListPath);
            ~FontManager();
            std::shared_ptr<GameFont> GetFont(const std::string fontName, const int fontSize);
        private:
            std::map<std::pair<std::string, int>, std::shared_ptr<GameFont>> fonts;

            bool LoadFonts(const std::string fontAssetListPath);
        };
    }
}


#endif
