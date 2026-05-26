#ifndef SDL03_Game_Scene_Dialogue_Choice
#define SDL03_Game_Scene_Dialogue_Choice

#include <memory>
#include <string>

namespace Game {
    namespace Scene {
        namespace Dialogue {
            class DialogueNode;

            class DialogueChoice {
            public:
                DialogueChoice(const std::string text, std::shared_ptr<DialogueNode> next);

                std::string text;
                std::shared_ptr<DialogueNode> next;
            };
        }
    }
}

#endif
