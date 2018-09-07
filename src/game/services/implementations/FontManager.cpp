#include "FontManager.h"

namespace Services {
    namespace Implementations {
        FontManager::FontManager(const std::string fontAssetListPath) {
            this->LoadFonts(fontAssetListPath);
        }

        FontManager::~FontManager() {
        }

        std::shared_ptr<GameFont> FontManager::GetFont(const std::string fontName, const int fontSize = GameFont::DEFAULT_FONT_SIZE) {
            std::map<std::pair<std::string, int>, std::shared_ptr<GameFont>>::iterator font = this->fonts.find(std::make_pair(fontName, fontSize));

            if (font != this->fonts.end()) {
                return font->second;
            }

            std::shared_ptr<GameFont> defaultSizeFont = this->fonts.at(std::make_pair(fontName, GameFont::DEFAULT_FONT_SIZE));

            const std::pair<std::string, int> fontKey = std::make_pair(fontName, fontSize);

            this->fonts[fontKey] = std::make_shared<GameFont>(defaultSizeFont->filename, fontSize);

            return this->fonts[fontKey];
        }

        bool FontManager::LoadFonts(const std::string fontAssetListPath) {
            // TODO: Figure out how to handle logging within services.
            // If this ends up going the route of having logging be a service,
            // which I still think is a good idea, then there needs to be some
            // sort of way to ensure that services are available before being
            // called.
            // this->logger->debug() << "Loading fonts from asset list \"" << assetListPath << "\".";

            std::string jsonString;

            if (!FileSystemHelpers::ReadFile(fontAssetListPath, jsonString)) {
                // this->logger->error() << "Failed to load asset list \"" << assetListPath << "\".";

                return false;
            }

            AssetListParser parser = AssetListParser();
            std::map<std::string, std::string> assetList;

            parser.Parse(jsonString, &assetList);

            // this->logger->debug() << "parsed asset list \"" << assetListPath << "\".";

            for (std::map<std::string, std::string>::iterator fontFilename = assetList.begin(); fontFilename != assetList.end(); fontFilename++) {

                this->fonts[std::make_pair(fontFilename->first, GameFont::DEFAULT_FONT_SIZE)] = std::make_shared<GameFont>(fontFilename->second, GameFont::DEFAULT_FONT_SIZE);
            }

            return true;
        }
    }
}
