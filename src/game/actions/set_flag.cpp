#include "set_flag.hpp"

namespace Game {
    namespace Actions {
        SetFlag::SetFlag(const std::string& key, bool value) : key(key), value(value), completed(false) {
        }

        SetFlag::~SetFlag() {
        }

        void SetFlag::Start() {
            Services::Locator::WorldService()->GetState()->flags.Set(this->key, this->value);

            this->completed = true;
        }

        void SetFlag::Update(float deltaTime) {
        }

        bool SetFlag::IsCompleted() const {
            return this->completed;
        }
    }
}
