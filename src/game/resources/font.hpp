#ifndef SDL03_Game_Resources_Font
#define SDL03_Game_Resources_Font

#include "SDL_ttf.h"
#include "base.hpp"

namespace Game {
    namespace Resources {
        class Font : public Base {
        public:
            //friend class Services::Implementations::FontManager;

            static const std::string DEFAULT_FONT_NAME;
            static const int DEFAULT_FONT_SIZE;

            TTF_Font* font;

            Font();
            Font(const std::string& filename);
            Font(const std::string& filename, const int fontSize);
            ~Font();
            void Load(const std::string& filename);
            void Load(const std::string& filename, const int fontSize);
            const std::string GetFilename();

        private:
            static const std::string logChannel;

            std::string filename;
            int pointSize;

            bool DestroyFont();
        };
    }
}

#endif
