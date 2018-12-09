#include "font.hpp"

namespace Game {
    namespace Resources {
        const std::string Font::DEFAULT_FONT_NAME = "DroidSans";
        const int Font::DEFAULT_FONT_SIZE = 10;
        const std::string Font::logChannel = "assets.fonts";

        Font::Font() {
            this->logger = Services::Locator::LoggerService()->GetLogger(Font::logChannel);
            this->font = nullptr;
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

            this->font = TTF_OpenFont(filename.c_str(), this->pointSize);

            if (this->font != nullptr) {
                this->logger->debug() << "Loaded font \"" << filename << "\" at size " << pointSize << ".";
            } else {
                this->logger->error() << "Failed to loaded font \"" << filename << "\" at size " << pointSize << ".";

                throw;
            }
        }

        TTF_Font* Font::GetTTFFont() {
            return this->font;
        }

        bool Font::DestroyTTFFont() {
            if (this->font == nullptr) {
                return false;
            }

            this->logger->debug() << "Destroying font \"" << this->filename << "\" at size " << this->pointSize << ".";

            TTF_CloseFont(this->font);

            this->logger->debug() << "Destroyed font \"" << this->filename << "\" at size " << this->pointSize << ".";

            this->font = nullptr;

            return true;
        }
    }
}
