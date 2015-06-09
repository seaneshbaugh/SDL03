#ifndef __SDL03__GameInputMapper__
#define __SDL03__GameInputMapper__

#include <map>

#include "SDL.h"

#define NO_KEY        0
#define UP_KEY        1
#define DOWN_KEY      2
#define LEFT_KEY      3
#define RIGHT_KEY     4
#define CONFIRM_KEY   5
#define CANCEL_KEY    6
#define MENU_KEY      7

class GameInputMapper {
public:
    GameInputMapper();
    ~GameInputMapper();
    std::map<int, int>::size_type MapKeys(std::map<int, int> keys);
    int SetInputMapKey(int rawKeyValue, int inputValue);
    int GetInputMapKey(SDL_Event* event);
private:
    std::map<int, int> inputMap;
};

#endif