#include "main.h"

int main(int argc, char* args[]) {
    GameEngine engine;
    
    if (!engine.Setup()) {
        return 1;
    }

    engine.Start();

    return 0;
}
