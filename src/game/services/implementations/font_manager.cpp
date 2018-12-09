#include "../../resources/font.hpp"
#include "font_manager.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            const std::string FontManager::logChannel = "fonts";

            FontManager::FontManager(const std::string& fontAssetListPath) {
                this->logger = Locator::LoggerService()->GetLogger(FontManager::logChannel);

                if (TTF_Init() == -1) {
                    this->logger->critic() << "Failed to initialize SDL_ttf. " << TTF_GetError();

                    throw;
                }

                this->logger->info() << "SDL_ttf initialized.";


                this->LoadFonts(fontAssetListPath);
            }

            FontManager::~FontManager() {
                for (auto it = this->fonts.begin(); it != this->fonts.end(); ++it) {
                    it->second->DestroyTTFFont();
                }

                this->logger->debug() << "Quitting SDL_ttf.";

                TTF_Quit();
            }

            std::shared_ptr<Resources::Font> FontManager::GetFont(const std::string& fontName, const int fontSize = Resources::Font::DEFAULT_FONT_SIZE) {
                auto font = this->fonts.find(std::make_pair(fontName, fontSize));

                if (font != this->fonts.end()) {
                    return font->second;
                }

                std::shared_ptr<Resources::Font> defaultSizeFont = this->fonts.at(std::make_pair(fontName, Resources::Font::DEFAULT_FONT_SIZE));

                const std::pair<std::string, int> fontKey = std::make_pair(fontName, fontSize);

                this->fonts[fontKey] = std::make_shared<Resources::Font>(defaultSizeFont->GetFilename(), fontSize);

                return this->fonts[fontKey];
            }

            bool FontManager::LoadFonts(const std::string& fontAssetListPath) {
                // TODO: Figure out how to handle logging within services.
                // If this ends up going the route of having logging be a service,
                // which I still think is a good idea, then there needs to be some
                // sort of way to ensure that services are available before being
                // called.
                // this->logger->debug() << "Loading fonts from asset list \"" << assetListPath << "\".";

                std::string jsonString;

                if (!Helpers::FileSystem::ReadFile(fontAssetListPath, jsonString)) {
                    // this->logger->error() << "Failed to load asset list \"" << assetListPath << "\".";

                    return false;
                }

                Parsers::AssetListParser parser = Parsers::AssetListParser();
                std::map<std::string, std::string> assetList = parser.Parse(jsonString);

                // this->logger->debug() << "parsed asset list \"" << assetListPath << "\".";

                for (std::map<std::string, std::string>::iterator fontFilename = assetList.begin(); fontFilename != assetList.end(); fontFilename++) {

                    this->fonts[std::make_pair(fontFilename->first, Resources::Font::DEFAULT_FONT_SIZE)] = std::make_shared<Resources::Font>(fontFilename->second, Resources::Font::DEFAULT_FONT_SIZE);
                }

                return true;
            }
        }
    }
}
