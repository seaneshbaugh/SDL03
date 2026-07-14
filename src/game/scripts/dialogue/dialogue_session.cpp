#include "dialogue_session.hpp"

namespace Game {
    namespace Scripts {
        namespace Dialogue {
            DialogueSession::DialogueSession() : selectedChoice(0), completed(false), characterTimer(0.0f), nextIndicatorVisible(false), nextIndicatorTimer(0.0f), inputDebounceTimer(0.0f) {
                this->backgroundTexture = Services::Locator::TextureService()->AddTexture("ui/dialogue_box", "assets/images/ui/battle/menu/background.png");
                this->nextIndicatorTexture = Services::Locator::TextureService()->AddTexture("ui/dialogue_next_indicator", "assets/images/ui/dialogue/more.png");
                this->choiceIndicatorTexture = Services::Locator::TextureService()->AddTexture("ui/dialogue_choice_indicator", "assets/images/ui/cursor-right.png");
            }

            DialogueSession::~DialogueSession() {
            }

            void DialogueSession::Start(const DialoguePage& page, const bool nextIndicatorVisible) {
                this->page = page;
                this->selectedChoice = 0;
                this->completed = false;
                this->characterTimer = 0.0f;
                this->nextIndicatorTimer = 0.0f;
                this->nextIndicatorVisible = nextIndicatorVisible;
                this->inputDebounceTimer = 0.0f;
                this->lines = Helpers::String::Split(this->page.text, "\n");
                this->visibleText.clear();

                for (std::vector<std::string>::size_type i = 0; i < this->lines.size(); ++i) {
                    this->visibleText.push_back("");
                }
            }

            void DialogueSession::HandleInput(const float deltaTime) {
                if (Services::Locator::InputService()->GetCurrentInputState().upHeld) {
                    if (this->inputDebounceTimer <= 0.0f) {
                        this->PreviousChoice();

                        this->inputDebounceTimer = 0.25f;
                    } else {
                        this->inputDebounceTimer -= deltaTime;
                    }
                } else if (Services::Locator::InputService()->GetCurrentInputState().downHeld) {
                    if (this->inputDebounceTimer <= 0.0f) {
                        this->NextChoice();

                        this->inputDebounceTimer = 0.25f;
                    } else {
                        this->inputDebounceTimer -= deltaTime;
                    }
                } else {
                    this->inputDebounceTimer = 0.0f;
                }

                if (Services::Locator::InputService()->GetCurrentInputState().confirmPressed) {
                    this->Next();
                }
            }

            void DialogueSession::PreviousChoice() {
                if (this->page.choices.empty()) {
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
                    this->selectedChoice = this->page.choices.size() - 1;
                }
            }

            void DialogueSession::NextChoice() {
                if (this->page.choices.empty()) {
                    return;
                }

                for (std::vector<std::string>::size_type i = 0; i < this->lines.size(); ++i) {
                    if (this->visibleText[i].size() != this->lines[i].size()) {
                        return;
                    }
                }

                if (this->selectedChoice < this->page.choices.size() - 1) {
                    this->selectedChoice++;
                } else {
                    this->selectedChoice = 0;
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
                    this->completed = true;
                }
            }

            void DialogueSession::Update(const float deltaTime) {
                if (this->completed) {
                    return;
                }

                this->HandleInput(deltaTime);

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

            void DialogueSession::Render(std::shared_ptr<Scenes::Camera> camera) {
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

                if (this->page.choices.empty()) {
                    if (allTextVisible && this->nextIndicatorTexture && this->nextIndicatorVisible && this->nextIndicatorTimer < 0.5f) {
                        SDL_FRect srcrect = {0.0f, 0.0f, static_cast<float>(this->nextIndicatorTexture->GetSDLTexture().get()->w), static_cast<float>(this->nextIndicatorTexture->GetSDLTexture().get()->h)};
                        SDL_FRect dstrect = {camera->viewportWidth - 32.0f - srcrect.w, camera->viewportHeight - 16.0f - srcrect.h, srcrect.w, srcrect.h};

                        Services::Locator::VideoService()->RenderTexture(this->nextIndicatorTexture, &srcrect, &dstrect);
                    }
                } else {
                    if (allTextVisible) {
                        for (std::vector<std::string>::size_type i = 0; i < this->page.choices.size(); ++i) {
                            SDL_Color color = {255, 255, 255, 255};
                            SDL_FRect textDstRect = {64.0f, camera->viewportHeight - 180.0f + static_cast<float>(this->visibleText.size() + i) * 24.0f, 0.0f, 0.0f};
                            Objects::Text text = Objects::Text(this->page.choices[i].text, font, textDstRect.x, textDstRect.y, color);

                            text.Render();

                            if (i == this->selectedChoice && this->choiceIndicatorTexture) {
                                SDL_FRect srcrect = {0.0f, 0.0f, static_cast<float>(this->choiceIndicatorTexture->GetSDLTexture().get()->w), static_cast<float>(this->choiceIndicatorTexture->GetSDLTexture().get()->h)};
                                SDL_FRect dstrect = {textDstRect.x - 8.0f - srcrect.w, textDstRect.y + (srcrect.h / 2.0f) - 4.0f, srcrect.w, srcrect.h};

                                Services::Locator::VideoService()->RenderTexture(this->choiceIndicatorTexture, &srcrect, &dstrect);
                            }
                        }
                    }
                }
            }

            bool DialogueSession::IsCompleted() const {
                return this->completed;
            }

            std::string DialogueSession::GetSelectedChoice() const {
                return this->page.choices[this->selectedChoice].nextNodeId;
            }
        }
    }
}
