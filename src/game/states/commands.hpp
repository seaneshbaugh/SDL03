#ifndef SDL03_Game_States_Commands
#define SDL03_Game_States_Commands

#include <memory>
#include <string>
#include <variant>

namespace Game {
    namespace Scenes {
        class Actor;
    }

    namespace States {
        struct LoadMapCommand {
            std::string mapName;
            int startX;
            int startY;
        };

        struct StartInteractionCommand {
            Scenes::Actor* actor;
        };

        struct StartScriptCommand {
            std::string scriptId;
        };

        using StateCommand = std::variant<
            LoadMapCommand,
            StartInteractionCommand,
            StartScriptCommand
        >;
    }
}

#endif
