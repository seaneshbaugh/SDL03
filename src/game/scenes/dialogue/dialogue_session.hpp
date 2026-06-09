#ifndef SDL03_Game_Scene_Dialogue_Session
#define SDL03_Game_Scene_Dialogue_Session

#include "../../services/locator.hpp"
#include "../../assets/texture.hpp"
#include "../../assets/font.hpp"
#include "../../objects/text.hpp"
#include "../camera.hpp"
#include "dialogue_graph.hpp"
#include "../../../helpers/string.hpp"

namespace Game {
    namespace Scenes {
        namespace Dialogue {
            class DialogueSession {
            public:
                DialogueSession();
                ~DialogueSession();

                void Start(std::shared_ptr<DialogueGraph> graph);
                void Update(const float deltaTime);
                void Render(std::shared_ptr<Camera> camera);
                bool IsCompleted() const;

            private:
                std::shared_ptr<DialogueGraph> currentGraph;
                std::shared_ptr<DialogueNode> currentNode;
                std::shared_ptr<Assets::Texture> backgroundTexture;
                std::shared_ptr<Assets::Texture> nextIndicatorTexture;
                std::shared_ptr<Assets::Texture> choiceIndicatorTexture;
                std::vector<std::string> lines;
                std::vector<std::string> visibleText;
                int selectedChoice;
                float characterTimer;
                float nextIndicatorTimer;
                float inputDebounceTimer;
                bool completed;

                void HandleInput(const float deltaTime);
                void PreviousChoice();
                void NextChoice();

                void Next();

                void SetCurrentNode(std::shared_ptr<DialogueNode> node);
            };
        }
    }
}

#endif
