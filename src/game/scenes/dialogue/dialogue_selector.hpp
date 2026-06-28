#ifndef SDL03_Game_Scenes_Dialogue_Selector
#define SDL03_Game_Scenes_Dialogue_Selector

#include "dialogue_profile.hpp"
#include "../../world/conditions/contexts/context.hpp"

namespace Game {
    namespace Scenes {
        namespace Dialogue {
            class DialogueSelector {
            public:
                DialogueSelector(const std::string& dialogueProfileId);
                ~DialogueSelector();

                std::string SelectDialogue(const World::Conditions::Contexts::Context& context);

            private:
                DialogueProfile dialogueProfile;
            };
        }
    }
}

#endif
