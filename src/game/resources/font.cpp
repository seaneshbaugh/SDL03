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

        Font::Font(const std::string& filename, const int fontSize) : Font() {
            this->logger->debug() << "Loading font \"" << filename << "\" at size " << fontSize << ".";
            this->Load(filename, fontSize);
        }

        Font::~Font() {
            this->DestroyFont();
        }

        void Font::Load(const std::string& filename) {
            this->Load(filename, this->pointSize);
        }

        void Font::Load(const std::string& filename, const int pointSize) {
            if (this->font != nullptr) {
                TTF_CloseFont(this->font);

                this->font = nullptr;
            }

            this->filename = filename;

            this->pointSize = pointSize;

            this->logger->debug() << "Loading font \"" << filename << "\".";

            this->font = TTF_OpenFont(filename.c_str(), this->pointSize);

            if (this->font != nullptr) {
                this->logger->debug() << "Successfully loaded font at size " << this->pointSize << ".";
            } else {
                this->logger->error() << "Failed to loaded font.";
            }
        }

        const std::string Font::GetFilename() {
            return this->filename;
        }

        bool Font::DestroyFont() {
            if (this->font == nullptr) {
                return false;
            }

            this->logger->debug() << "Destroying font \"" << this->filename << "\".";

            TTF_CloseFont(this->font);

            this->logger->debug() << "Destroyed font \"" << this->filename << "\".";

            this->font = nullptr;

            return true;
        }
    }
}
