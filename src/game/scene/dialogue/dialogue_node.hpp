#ifndef SDL03_Game_Scene_Dialogue_Node
#define SDL03_Game_Scene_Dialogue_Node

#include <memory>
#include <string>
#include <vector>

#include "dialogue_choice.hpp"

namespace Game {
    namespace Scene {
        namespace Dialogue {
            class DialogueNode {
            public:
                enum class Type {
                    Text,
                    Choice
                };

                DialogueNode(const Type type, const std::string& key, const std::string& text);
                ~DialogueNode();

                Type type;
                std::string key;
                std::string text;
                std::shared_ptr<DialogueNode> next;
                std::vector<DialogueChoice> choices;
            private:
            };
        }
    }
}

#endif
