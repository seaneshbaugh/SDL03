#include "dialogue_session.hpp"

namespace Game {
    namespace Scene {
        namespace Dialogue {
            DialogueSession::DialogueSession() : selectedChoice(0), completed(false), characterTimer(0.0f), nextIndicatorTimer(0.0f) {
                this->backgroundTexture = Services::Locator::TextureService()->AddTexture("ui/dialogue_box", "assets/images/ui/battle/menu/background.png");
                this->nextIndicatorTexture = Services::Locator::TextureService()->AddTexture("ui/dialogue_next_indicator", "assets/images/ui/dialogue/more.png");
                this->choiceIndicatorTexture = Services::Locator::TextureService()->AddTexture("ui/dialogue_choice_indicator", "assets/images/ui/cursor-right.png");
            }

            DialogueSession::~DialogueSession() {
            }

            void DialogueSession::Start(std::shared_ptr<DialogueGraph> graph) {
                this->currentGraph = graph;
                this->currentNode = graph->root;
                this->selectedChoice = 0;
                this->completed = false;
                this->characterTimer = 0.0f;
                this->nextIndicatorTimer = 0.0f;
                this->lines = Helpers::String::Split(this->currentNode->text, "\n");
                this->visibleText.clear();

                for (std::vector<std::string>::size_type i = 0; i < this->lines.size(); ++i) {
                    this->visibleText.push_back("");
                }
            }

            void DialogueSession::Next() {
                bool allTextVisible = true;

                for (std::vector<std::string>::size_type i = 0; i < this->lines.size(); ++i) {
                    if (this->visibleText[i].size() != this->lines[i].size()) {
                        allTextVisible = false;

                        break;
                    }
                }

                if (!allTextVisible) {
                    for (std::vector<std::string>::size_type i = 0; i < this->lines.size(); ++i) {
                        this->visibleText[i] = this->lines[i];
                    }
                } else {
                    if (this->currentNode->type == DialogueNode::Type::Text) {
                        if (this->currentNode->next) {
                            this->SetCurrentNode(this->currentNode->next);
                        } else {
                            this->completed = true;
                        }
                    } else if (this->currentNode->type == DialogueNode::Type::Choice) {
                        if (this->currentNode->choices.size() > 0) {
                            std::shared_ptr<DialogueNode> selectedNode = this->currentNode->choices[this->selectedChoice].next;

                            if (selectedNode) {
                                // TODO: This is probably where we'll want to set game state flags based on the choice made.
                                this->SetCurrentNode(selectedNode);
                            } else {
                                this->completed = true;
                            }
                        } else {
                            this->completed = true;
                        }
                    }
                }
            }

            void DialogueSession::PreviousChoice() {
                if (this->currentNode->type != DialogueNode::Type::Choice) {
                    return;
                }

                for (std::vector<std::string>::size_type i = 0; i < this->lines.size(); ++i) {
                    if (this->visibleText[i].size() != this->lines[i].size()) {
                        return;
                    }
                }

                if (this->selectedChoice > 0) {
                    this->selectedChoice--;
                } else {
                    this->selectedChoice = this->currentNode->choices.size() - 1;
                }
            }

            void DialogueSession::NextChoice() {
                if (this->currentNode->type != DialogueNode::Type::Choice) {
                    return;
                }

                for (std::vector<std::string>::size_type i = 0; i < this->lines.size(); ++i) {
                    if (this->visibleText[i].size() != this->lines[i].size()) {
                        return;
                    }
                }

                if (this->selectedChoice < this->currentNode->choices.size() - 1) {
                    this->selectedChoice++;
                } else {
                    this->selectedChoice = 0;
                }
            }

            void DialogueSession::Update(const float deltaTime) {
                if (this->completed) {
                    return;
                }

                this->characterTimer += deltaTime;

                float characterTimerOffset = 0.0f;

                for (std::vector<std::string>::size_type i = 0; i < this->lines.size(); ++i) {
                    if (this->visibleText[i] != this->lines[i]) {
                        while (this->characterTimer >= 0.05f && this->visibleText[i].size() < this->lines[i].size()) {
                            this->visibleText[i] += this->lines[i][this->visibleText[i].size()];
                            this->characterTimer -= 0.05f;
                            this->nextIndicatorTimer = 0.0f;
                        }
                    } else {
                        continue;
                    }
                }

                if (this->nextIndicatorTimer < 1.0f) {
                    this->nextIndicatorTimer += deltaTime;
                } else {
                    this->nextIndicatorTimer = 0.0f;
                }
            }

            void DialogueSession::Render(std::shared_ptr<Camera> camera) {
                if (this->completed) {
                    return;
                }

                if (this->backgroundTexture) {
                    SDL_FRect srcrect = {0.0f, 0.0f, static_cast<float>(this->backgroundTexture->GetSDLTexture().get()->w), static_cast<float>(this->backgroundTexture->GetSDLTexture().get()->h)};
                    SDL_FRect dstrect = {0, camera->viewportHeight - 200.0f, srcrect.w, srcrect.h};

                    Services::Locator::VideoService()->RenderTexture(this->backgroundTexture, &srcrect, &dstrect);
                }

                bool allTextVisible = true;
                std::shared_ptr<Assets::Font> font = Services::Locator::FontService()->GetFont("PixChicago", 16);

                for (std::vector<std::string>::size_type i = 0; i < this->visibleText.size(); ++i) {
                    if (this->visibleText[i] != "") {
                        SDL_Color color = {255, 255, 255, 255};
                        SDL_FRect textDstRect = {32.0f, camera->viewportHeight - 180.0f + static_cast<float>(i) * 24.0f, 0.0f, 0.0f};
                        Objects::Text text = Objects::Text(this->visibleText[i], font, textDstRect.x, textDstRect.y, color);

                        text.Render();
                    }

                    if (this->visibleText[i] != this->lines[i]) {
                        allTextVisible = false;
                    }
                }

                if (this->currentNode->type == DialogueNode::Type::Text) {
                    if (allTextVisible && this->nextIndicatorTexture && this->currentNode->next && this->nextIndicatorTimer < 0.5f) {
                        SDL_FRect srcrect = {0.0f, 0.0f, static_cast<float>(this->nextIndicatorTexture->GetSDLTexture().get()->w), static_cast<float>(this->nextIndicatorTexture->GetSDLTexture().get()->h)};
                        SDL_FRect dstrect = {camera->viewportWidth - 32.0f - srcrect.w, camera->viewportHeight - 16.0f - srcrect.h, srcrect.w, srcrect.h};

                        Services::Locator::VideoService()->RenderTexture(this->nextIndicatorTexture, &srcrect, &dstrect);
                    }
                } else if (this->currentNode->type == DialogueNode::Type::Choice) {
                    if (allTextVisible && this->currentNode->choices.size() > 0) {
                        for (std::vector<DialogueChoice>::size_type i = 0; i < this->currentNode->choices.size(); ++i) {
                            SDL_Color color = {255, 255, 255, 255};
                            SDL_FRect textDstRect = {64.0f, camera->viewportHeight - 180.0f + static_cast<float>(this->visibleText.size() + i) * 24.0f, 0.0f, 0.0f};
                            Objects::Text text = Objects::Text(this->currentNode->choices[i].text, font, textDstRect.x, textDstRect.y, color);

                            text.Render();

                            if (i == this->selectedChoice && this->choiceIndicatorTexture) {
                                SDL_FRect srcrect = {0.0f, 0.0f, static_cast<float>(this->choiceIndicatorTexture->GetSDLTexture().get()->w), static_cast<float>(this->choiceIndicatorTexture->GetSDLTexture().get()->h)};
                                SDL_FRect dstrect = {textDstRect.x - 8.0f - srcrect.w, textDstRect.y + (srcrect.h / 2.0f) - 4.0f /* + (textDstRect.h - srcrect.h) / 2.0f */, srcrect.w, srcrect.h};

                                Services::Locator::VideoService()->RenderTexture(this->choiceIndicatorTexture, &srcrect, &dstrect);
                            }
                        }
                    }
                }
            }

            void DialogueSession::SetCurrentNode(std::shared_ptr<DialogueNode> node) {
                this->currentNode = node;
                this->selectedChoice = 0;
                this->characterTimer = 0.0f;
                this->nextIndicatorTimer = 0.0f;
                this->lines = Helpers::String::Split(this->currentNode->text, "\n");
                this->visibleText.clear();

                for (const auto& line : this->lines) {
                    this->visibleText.push_back("");
                }
            }
        }
    }
}
