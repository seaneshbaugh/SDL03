#include <string>
#include "SDL.h"
#include "engine.h"

int main(int argc, char* args[]) {
    Engine engine;
    
    if (!engine.Setup()) {
        return 1;
    }

    engine.Start();

    return 0;
}
