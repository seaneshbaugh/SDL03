#ifndef SDL03_Game_Resources_Font
#define SDL03_Game_Resources_Font

#include <SDL_ttf.h>

#include "base.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            class FontManager;
        }
    }

    namespace Resources {
        class Font : public Base {
        public:
            friend class Services::Implementations::FontManager;

            static const std::string DEFAULT_FONT_NAME;
            static const int DEFAULT_FONT_SIZE;

            Font();
            Font(const std::string& filename);
            Font(const std::string& filename, const int pointSize);
            ~Font();
            void Load(const std::string& filename);
            void Load(const std::string& filename, const int pointSize);
            std::shared_ptr<TTF_Font> GetTTFFont();

        protected:
            bool DestroyTTFFont();

        private:
            static const std::string logChannel;

            std::shared_ptr<TTF_Font> ttfFont;
            int pointSize;
        };
    }
}

#endif
