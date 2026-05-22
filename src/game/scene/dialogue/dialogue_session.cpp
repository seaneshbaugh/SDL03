#include "dialogue_session.hpp"

namespace Game {
    namespace Scene {
        namespace Dialogue {
            DialogueSession::DialogueSession() : selectedChoice(0), completed(false), visibleText(""), characterTimer(0.0f) {
            }

            DialogueSession::~DialogueSession() {
            }
        }
    }
}
