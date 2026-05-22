#ifndef SDL03_Game_Scene_Dialogue_Node
#define SDL03_Game_Scene_Dialogue_Node

#include <string>
#include <vector>
#include <memory>

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
                std::vector<std::shared_ptr<DialogueNode>> choices;
            private:
            };
        }
    }
}

#endif
