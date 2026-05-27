#include "dialogue.hpp"
#include "../../../states/map.hpp"

namespace Game {
    namespace Scene {
        namespace Cutscenes {
            namespace Actions {
                Dialogue::Dialogue(States::Map* map, const std::string& dialogueId) : map(map), dialogueId(dialogueId), started(false) {
                }

                void Dialogue::Start() {
                    // I think what I'll need is a way to keep track of the previous substate so I can return to it after the dialogue is done.
                    // That'll make it so if you're talking to an NPC it'll go back to the Gameplay state but if you're in the middle of a
                    // cutscene it'll got back to the cutscene substate and then continue on with the rest of the cutscene.
                    // The biggest problem wiht this is that it means that dialogue will freeze the cutscene while it's happening. Which is
                    // definitely not what I want. I think I might have to make it so that the cutscene state can handle input for dialogue
                    // if the current action is a dialogue action. Not sure about this all right now. I mostly want to just get dialogue
                    // working in the middle of a cutscene and then figure out how to make it so that the cutscene can keep going while
                    // dialogue is happening later.
                    this->map->StartDialogue(this->dialogueId);

                    this->started = true;
                }

                void Dialogue::Update(float deltaTime) {
                }

                bool Dialogue::IsCompleted() const {
                    return true;
                }
            }
        }
    }
}
