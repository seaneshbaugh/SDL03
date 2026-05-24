#include "dialogue_session.hpp"

namespace Game {
    namespace Scene {
        namespace Dialogue {
            DialogueSession::DialogueSession() : selectedChoice(0), completed(false), visibleText(""), characterTimer(0.0f) {
                this->backgroundTexture = Services::Locator::TextureService()->AddTexture("ui/dialogue_box", "assets/images/ui/battle/menu/background.png");
            }

            DialogueSession::~DialogueSession() {
            }

            void DialogueSession::Start(std::shared_ptr<DialogueGraph> graph) {
                this->currentGraph = graph;
                this->currentNode = graph->root;
                this->selectedChoice = 0;
                this->completed = false;
                this->visibleText = "";
                this->characterTimer = 0.0f;
               // this->lines = Helpers::String::Split(this->currentNode->text, "\n");


            }

            void DialogueSession::Next() {
                if (this->currentNode->type == DialogueNode::Type::Text) {
                    if (this->visibleText.size() < this->currentNode->text.size()) {
                        this->visibleText = this->currentNode->text;
                    } else {
                        if (this->currentNode->next) {
                            this->currentNode = this->currentNode->next;
                            this->selectedChoice = 0;
                            this->visibleText = "";
                            this->characterTimer = 0.0f;
                        } else {
                            this->completed = true;
                        }
                    }
                } else if (this->currentNode->type == DialogueNode::Type::Choice) {
                    if (this->currentNode->choices.size() > 0) {
                        std::shared_ptr<DialogueNode> selectedNode = this->currentNode->choices[this->selectedChoice];
                        if (selectedNode) {
                            this->currentNode = selectedNode;
                            this->selectedChoice = 0;
                            this->visibleText = "";
                            this->characterTimer = 0.0f;
                        } else {
                            this->completed = true;
                        }
                    } else {
                        this->completed = true;
                    }
                }
            }

            void DialogueSession::Update(const double deltaTime) {
                if (this->completed) {
                    return;
                }

                if (this->currentNode->type == DialogueNode::Type::Text) {
                    this->characterTimer += static_cast<float>(deltaTime);

                    while (this->characterTimer >= 0.05f && this->visibleText.size() < this->currentNode->text.size()) {
                        this->visibleText += this->currentNode->text[this->visibleText.size()];
                        this->characterTimer -= 0.05f;
                    }
                }
            }

            void DialogueSession::Render(std::shared_ptr<Camera> camera) {
                if (this->completed) {
                    return;
                }
                if (this->currentNode->type == DialogueNode::Type::Text) {
                    if (this->backgroundTexture) {
                        SDL_FRect srcrect = {0.0f, 0.0f, static_cast<float>(this->backgroundTexture->GetSDLTexture().get()->w), static_cast<float>(this->backgroundTexture->GetSDLTexture().get()->h)};
                        SDL_FRect dstrect = {0, camera->viewportHeight - 196.0f, srcrect.w, srcrect.h};
                        Services::Locator::VideoService()->RenderTexture(this->backgroundTexture, &srcrect, &dstrect);
                    }

                    if (this->visibleText != "") {
                        std::shared_ptr<Assets::Font> font = Services::Locator::FontService()->GetFont("PixChicago", 16);
                        SDL_Color color = {255, 255, 255, 255};
                        SDL_FRect textDstRect = {32.0f, camera->viewportHeight - 180.0f, 0.0f, 0.0f};
                        Objects::Text text = Objects::Text(this->visibleText, font, textDstRect.x, textDstRect.y, color);
                        text.Render();
                    }
                }
            }
        }
    }
}
