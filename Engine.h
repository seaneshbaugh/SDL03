#ifndef __SDL03__Engine__
#define __SDL03__Engine__

#include <string>
#include <vector>
#include "SDL.h"
#include "SDL_ttf.h"
#include "GameState.h"
#include "IntroState.h"
#include "GameResource.h"
#include "GameTexture.h"
#include "GameFont.h"
#include "GameSound.h"

class Engine {
public:
    Engine();
    ~Engine();
    bool Setup();
    void Start();
    // These should probably be behind getter methods.
    SDL_Window *screen;
    SDL_Renderer *renderer;

private:
    int screenWidth;
    int screenHeight;
    std::string windowTitle;
    std::vector <GameState*> states;
    std::vector <GameTexture*> textures;
    std::vector <GameFont*> fonts;
    std::vector <GameSound*> sounds;

    bool LoadResources();
    bool LoadTextures();
    bool LoadFonts();
    bool LoadSounds();
    void MainLoop();
    void Render();
};

#endif
