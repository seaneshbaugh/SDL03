#ifndef SDL03_Game_Scene_Dialogue_Session
#define SDL03_Game_Scene_Dialogue_Session

#include <string>

#include "../../services/locator.hpp"
#include "../../assets/texture.hpp"
#include "../../assets/font.hpp"
#include "../../objects/text.hpp"
#include "../camera.hpp"
#include "dialogue_graph.hpp"
#include "../../../helpers/string.hpp"

namespace Game {
    namespace Scene {
        namespace Dialogue {
            class DialogueSession {
            public:
                DialogueSession();
                ~DialogueSession();

                void Start(std::shared_ptr<DialogueGraph> graph);
                void Next();
                void Update(const double deltaTime);
                void Render(std::shared_ptr<Camera> camera);

                int selectedChoice;
                bool completed;
                std::string visibleText;
                float characterTimer;
                std::shared_ptr<DialogueGraph> currentGraph;
                std::shared_ptr<DialogueNode> currentNode;
                std::shared_ptr<Assets::Texture> backgroundTexture;

            private:
                std::vector<std::string> lines;
            };
        }
    }
}

#endif
