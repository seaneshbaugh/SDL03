#ifndef __SDL03__GameFont__
#define __SDL03__GameFont__

#include "SDL_ttf.h"

#include "LoggerCpp.h"

#include "GameResource.h"

namespace Services {
    namespace Implementations {
        class FontManager;
    }
}

class GameFont : public GameResource {
public:
    friend class Services::Implementations::FontManager;

    static const int DEFAULT_FONT_SIZE;

    TTF_Font* font;

    GameFont();
    GameFont(const std::string filename);
    GameFont(const std::string filename, const int fontSize);
    ~GameFont();
    bool Load(std::string filename);
    bool Load(std::string filename, int pointSize);

private:
    std::string filename;
    int pointSize;
    Log::Logger* logger;

    bool DestroyFont();
};

#endif
