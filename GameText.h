#ifndef __SDL03__GameText__
#define __SDL03__GameText__

#include <string>

#include "SDL_ttf.h"

#include "GameFont.h"
#include "GameObject.h"

// I'm not really sold on the idea of having all of the data be private and then
// having a bunch of setter functions. Since this isn't intended for use in other
// applications it doesn't really make sense. But I'm going to stick with it like
// this for now since I have a small hunch it might keep other things clean and
// clearly encapsulated. Mostly because I kinda want to ensure that any time any
// piece of data is changed the texture is re-made.
class GameText : public GameObject {
public:
    GameText(SDL_Renderer* renderer);
    GameText(SDL_Renderer* renderer, std::string text, GameFont* font, int x, int y, SDL_Color color);
    ~GameText();
    void SetText(std::string text);
    void SetFont(GameFont* font);
    void SetPosition(int x, int y);
    void SetColor(SDL_Color color);
    void Render();
private:
    SDL_Renderer *renderer;
    std::string text;
    GameFont* font;
    int x;
    int y;
    SDL_Color color;
    SDL_Rect textLocation;
    SDL_Texture *texture;

    void UpdateTexture();
};

#endif
