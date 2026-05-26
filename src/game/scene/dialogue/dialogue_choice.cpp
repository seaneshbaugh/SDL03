#include "dialogue_choice.hpp"

namespace Game {
    namespace Scene {
        namespace Dialogue {
            DialogueChoice::DialogueChoice(const std::string text, std::shared_ptr<DialogueNode> next) : text(text), next(next) {
            }
        }
    }
}
