#ifndef SDL03_Game_Scene_Dialogue_Graph
#define SDL03_Game_Scene_Dialogue_Graph

#include "dialogue_node.hpp"

namespace Game {
    namespace Scene {
        namespace Dialogue {
            class DialogueGraph {
            public:
                DialogueGraph();
                ~DialogueGraph();

                std::shared_ptr<DialogueNode> root;
            private:
                // TODO: Parser goes here.
            };
        }
    }
}

#endif
