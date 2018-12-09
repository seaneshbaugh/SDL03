#ifndef SDL03_Game_Resources_Font
#define SDL03_Game_Resources_Font

#include "SDL_ttf.h"
#include "base.hpp"
#include "../services/implementations/font_manager.hpp"

namespace Game {
    namespace Resources {
        class Font : public Base {
        public:
            friend class Services::Implementations::FontManager;

            static const std::string DEFAULT_FONT_NAME;
            static const int DEFAULT_FONT_SIZE;

            TTF_Font* font;

            Font();
            Font(const std::string& filename);
            Font(const std::string& filename, const int pointSize);
            ~Font();
            void Load(const std::string& filename);
            void Load(const std::string& filename, const int pointSize);
            TTF_Font* GetTTFFont();

        protected:
            bool DestroyTTFFont();

        private:
            static const std::string logChannel;

            int pointSize;
        };
    }
}

#endif
