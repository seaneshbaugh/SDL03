#ifndef __SDL03__FontManager__
#define __SDL03__FontManager__

#include <map>

#include "FileSystemHelpers.h"

#include "AssetListParser.h"
#include "../interfaces/FontService.h"

namespace Services {
    namespace Implementations {
        class FontManager : public Interfaces::FontService {
        public:
            FontManager(const std::string fontAssetListPath);
            ~FontManager();
            std::shared_ptr<GameFont> GetFont(const std::string fontName);
            // GameFont* GetFont(const std::string fontName);
        private:
            std::map<std::string, std::shared_ptr<GameFont>> fonts;
            // std::map<std::string, GameFont*> fonts;
            std::map<std::string, int> fontSizes;

            bool LoadFonts(const std::string fontAssetListPath);
        };
    }
}


#endif
