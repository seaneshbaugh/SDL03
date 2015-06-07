#ifndef __SDL03__GameParty__
#define __SDL03__GameParty__

#include <vector>

#include "GameObject.h"
#include "GameCharacter.h"

class GameParty : public GameObject {
public:
    std::vector<GameCharacter*> characters;

    GameParty();
    ~GameParty();
};

#endif
