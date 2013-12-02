#include "main.h"

int main(int argc, char* args[]) {
    char resourceDirectory[MAXPATHLEN];

    if (CFURLGetFileSystemRepresentation(CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle()), 1, (UInt8*)resourceDirectory, MAXPATHLEN)) {
        chdir(resourceDirectory);
    }

    GameEngine engine;

    if (!engine.Setup()) {
        return 1;
    }

    engine.Start();

    return 0;
}
