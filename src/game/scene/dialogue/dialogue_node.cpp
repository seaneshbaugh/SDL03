#include "dialogue_node.hpp"

namespace Game {
    namespace Scene {
        namespace Dialogue {
            DialogueNode::DialogueNode(const Type type, const std::string& key, const std::string& text) : type(type), key(key), text(text), next(nullptr) {
            }

            DialogueNode::~DialogueNode() {
            }
        }
    }
}
