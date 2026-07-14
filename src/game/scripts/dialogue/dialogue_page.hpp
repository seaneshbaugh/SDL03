#ifndef SDL03_Game_Scripts_Dialogue_DialoguePage
#define SDL03_Game_Scripts_Dialogue_DialoguePage

#include <memory>
#include <string>
#include <vector>

namespace Game {
    namespace Scripts {
        class ScriptNode;

        namespace Dialogue {
            struct DialogueChoice {
                std::string text;
                std::string nextNodeId;
            };

            class DialoguePage {
            public:
                DialoguePage();
                ~DialoguePage();

                std::string text;
                std::vector<DialogueChoice> choices;
            };
        }
    }
}

#endif
