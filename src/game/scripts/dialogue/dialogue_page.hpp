#ifndef SDL03_Game_Scripts_Dialogue_DialoguePage
#define SDL03_Game_Scripts_Dialogue_DialoguePage

#include <map>
#include <memory>
#include <string>

namespace Game {
    namespace Scripts {
        namespace Dialogue {
            class DialoguePage {
            public:
                DialoguePage();
                ~DialoguePage();

                std::string text;
                std::map<std::string, std::string> choices;
            };
        }
    }
}

#endif
