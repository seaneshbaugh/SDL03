#ifndef __SDL03__GameCharacter__
#define __SDL03__GameCharacter__

#include <string>

#include "GameObject.h"

class GameCharacter : GameObject {
public:
    std::string name;
    unsigned long long level;
    unsigned long long hitPoints;
    unsigned long long magicPoints;

    GameCharacter();
    ~GameCharacter();
};

#endif
