#ifndef SDL03_Game_States_Transition
#define SDL03_Game_States_Transition

namespace Game {
    namespace States {
        class Base;

        struct Transition {
            enum class Type {
                None,
                Push,
                Pop,
                Replace,
                Quit
            };

            static Transition None() {
                return Transition{Type::None, nullptr};
            }

            static Transition Push(std::shared_ptr<Base> nextState) {
                return Transition{Type::Push, nextState};
            }

            static Transition Pop() {
                return Transition{Type::Pop, nullptr};
            }

            static Transition Replace(std::shared_ptr<Base> nextState) {
                return Transition{Type::Replace, nextState};
            }

            static Transition Quit() {
                return Transition{Type::Quit, nullptr};
            }

            Type type;
            std::shared_ptr<Base> nextState;
        };
    }
}

#endif
