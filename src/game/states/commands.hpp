#ifndef SDL03_Game_States_Commands
#define SDL03_Game_States_Commands

#include <string>
#include <variant>

namespace Game {
    namespace States {
        struct LoadMapCommand {
            std::string mapName;
            int startX;
            int startY;
        };

        struct StartDialogueCommand {
            std::string dialogueId;
        };

        struct StartCutsceneCommand {
            std::string cutsceneId;
        };

        using StateCommand = std::variant<
            LoadMapCommand,
            StartDialogueCommand,
            StartCutsceneCommand
        >;
    }
}

#endif
