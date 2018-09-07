#include "FontManager.h"

namespace Services {
    namespace Implementations {
        FontManager::FontManager(const std::string fontAssetListPath) {
            this->LoadFonts(fontAssetListPath);
        }

        FontManager::~FontManager() {
//            for (std::map<std::string, <std::shared_ptr<GameFont*>>::iterator font = this->fonts.begin(); font != fonts.end(); font++) {
//            // for (std::map<std::string, GameFont*>::iterator font = this->fonts.begin(); font != fonts.end(); font++) {
//                delete font->second;
//            }
        }

        std::shared_ptr<GameFont> FontManager::GetFont(const std::string fontName) {
        // GameFont* FontManager::GetFont(const std::string fontName) {
            return this->fonts[fontName];
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
                // std::shared_ptr<GameFont> font = std::make_shared<GameFont>(fontFilename->second, 10);

//                int fontSize;
//
//                try {
//                    fontSize = this->fontSizes.at(fontFilename->first);
//                } catch (const std::out_of_range& exception) {
//                    fontSize = 10;
//                }
//
//                if (!font->Load(fontFilename->second, fontSize)) {
//                    // this->logger->error() << "Failed to load font file \"" << fontFilename->second << "\".";
//
//                    return false;
//                }

                this->fonts[fontFilename->first] = std::make_shared<GameFont>(fontFilename->second, 10);
                // this->fonts[fontFilename->first] = new GameFont(fontFilename->second, 10);
            }

            return true;
        }
    }
}
