#include "font.hpp"

namespace Game {
    namespace Resources {
        const std::string Font::DEFAULT_FONT_NAME = "DroidSans";
        const int Font::DEFAULT_FONT_SIZE = 10;
        const std::string Font::logChannel = "assets.fonts";

        Font::Font() {
            this->logger = Services::Locator::LoggerService()->GetLogger(Font::logChannel);
            this->ttfFont = nullptr;
            this->filename = "";
            this->pointSize = Font::DEFAULT_FONT_SIZE;
        }

        Font::Font(const std::string& filename) : Font() {
            this->Load(filename);
        }

        Font::Font(const std::string& filename, const int pointSize) : Font() {
            this->Load(filename, pointSize);
        }

        Font::~Font() {
            this->DestroyTTFFont();
        }

        void Font::Load(const std::string& filename) {
            this->Load(filename, this->pointSize);
        }

        void Font::Load(const std::string& filename, const int pointSize) {
            this->DestroyTTFFont();

            this->filename = filename;

            this->pointSize = pointSize;

            this->logger->debug() << "Loading font \"" << filename << "\" at size " << pointSize << ".";

            this->ttfFont = std::shared_ptr<TTF_Font>(TTF_OpenFont(filename.c_str(), this->pointSize), TTF_CloseFont);

            if (this->ttfFont != nullptr) {
                this->logger->debug() << "Loaded font \"" << filename << "\" at size " << pointSize << ".";
            } else {
                this->logger->error() << "Error loading font \"" << filename << "\" at size " << pointSize << ": " << TTF_GetError();

                throw;
            }
        }

        std::shared_ptr<TTF_Font> Font::GetTTFFont() {
            return this->ttfFont;
        }

        bool Font::DestroyTTFFont() {
            if (this->ttfFont == nullptr) {
                return false;
            }

            this->logger->debug() << "Destroying font \"" << this->filename << "\" at size " << this->pointSize << ".";

            this->ttfFont.reset();

            this->logger->debug() << "Destroyed font \"" << this->filename << "\" at size " << this->pointSize << ".";

            return true;
        }
    }
}
