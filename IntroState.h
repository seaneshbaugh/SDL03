#ifndef __SDL03__IntroState__
#define __SDL03__IntroState__

#include "GameState.h"

class IntroState : public GameState {
public:
    IntroState(SDL_Renderer* renderer);
    ~IntroState();
    GameState* Update(SDL_Event* event);
    void ProcessInput(SDL_Event* event);
    void RenderObjects(std::map <std::string, GameTexture*> textures, std::map <std::string, GameFont*> fonts, std::map <std::string, GameSound*> sounds);
private:
    SDL_Renderer* renderer;
    bool pop;
};

#endif
