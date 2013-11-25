#include <string>
#include "SDL.h"
#include "engine.h"

int main(int argc, char* args[]) {
    Engine engine;
    
    if (!engine.setup()) {
        return 1;
    }

    engine.start();

    return 0;
}
