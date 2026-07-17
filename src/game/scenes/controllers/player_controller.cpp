#include "player_controller.hpp"

namespace Game {
    namespace Scenes {
        namespace Controllers {
            PlayerController::PlayerController(Actor* actor) : actor(actor) {
            }

            PlayerController::~PlayerController() {
            }

            void PlayerController::Update(float deltaTime) {
                Input::InputState inputState = Services::Locator::InputService()->GetCurrentInputState();

                bool movementInputHeld = inputState.upHeld || inputState.rightHeld || inputState.downHeld || inputState.leftHeld;

                if (!this->actor->IsMoving() && movementInputHeld) {
                    this->actor->ClearPendingMovement();

                    Direction direction;

                    if (inputState.upHeld) {
                        direction = Direction::Up;
                    } else if (inputState.rightHeld) {
                        direction = Direction::Right;
                    } else if (inputState.downHeld) {
                        direction = Direction::Down;
                    } else if (inputState.leftHeld) {
                        direction = Direction::Left;
                    }

                    this->actor->QueueMovement(direction);
                }

                if (inputState.confirmPressed) {
                    actor->QueueInteraction();
                }
            }
        }
    }
}
