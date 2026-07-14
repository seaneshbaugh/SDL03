#ifndef SDL03_Game_Scripts_Dialogue_DialogueSession
#define SDL03_Game_Scripts_Dialogue_DialogueSession

#include "../../../helpers/string.hpp"
#include "../../assets/font.hpp"
#include "../../assets/texture.hpp"
#include "../../objects/text.hpp"
#include "../../scenes/camera.hpp"
#include "../../services/locator.hpp"
#include "dialogue_page.hpp"

namespace Game {
    namespace Scripts {
        namespace Dialogue {
            class DialogueSession {
            public:
                DialogueSession();
                ~DialogueSession();

                void Start(const DialoguePage& page, const bool nextIndicatorVisible);
                void Update(const float deltaTime);
                void Render(std::shared_ptr<Scenes::Camera> camera);
                bool IsCompleted() const;
                std::string GetSelectedChoice() const;

            private:
                DialoguePage page;
                std::shared_ptr<Assets::Texture> backgroundTexture;
                std::shared_ptr<Assets::Texture> nextIndicatorTexture;
                std::shared_ptr<Assets::Texture> choiceIndicatorTexture;
                std::vector<std::string> lines;
                std::vector<std::string> visibleText;
                std::vector<DialogueChoice>::size_type selectedChoice;
                float characterTimer;
                bool nextIndicatorVisible;
                float nextIndicatorTimer;
                float inputDebounceTimer;
                bool completed;

                void HandleInput(const float deltaTime);
                void PreviousChoice();
                void NextChoice();

                void Next();
            };
        }
    }
}

#endif
