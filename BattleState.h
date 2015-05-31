#ifndef __SDL03__BattleState__
#define __SDL03__BattleState__

#include "GameState.h"

class BattleState : public GameState {
public:
    BattleState(std::function<void(GameState*)> callback);
    ~BattleState();
    GameState* Update(int key);
    std::string ProcessInput(int key);
    void Render();
protected:
    lua_State *luaState;
};

#endif
