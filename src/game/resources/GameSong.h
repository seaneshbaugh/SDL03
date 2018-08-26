#ifndef __SDL03__GameSong__
#define __SDL03__GameSong__

#include "SDL_mixer.h"

#include "LoggerCpp.h"

#include "GameResource.h"

class GameSong : public GameResource {
public:
    Mix_Music* music;

    GameSong();
    GameSong(std::string filename);
    ~GameSong();
    bool Load(std::string filename);
private:
    std::string filename;
    Log::Logger* logger;

    bool DestroyMusic();
};

#endif
