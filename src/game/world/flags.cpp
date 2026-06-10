#include "flags.hpp"

namespace Game {
    namespace World {
        Flags::Flags() {
        }

        Flags::~Flags() {
        }

        bool Flags::Exists(const std::string& key) const {
            return this->values.find(key) != this->values.end();
        }

        bool Flags::Get(const std::string& key, bool defaultValue) const {
            if (this->values.find(key) != this->values.end()) {
                return this->values.at(key);
            }

            return defaultValue;
        }

        void Flags::Set(const std::string& key, bool value) {
            this->values[key] = value;
        }

        json Flags::AsJSON() const {
            json j;

            for (const auto& [key, value] : this->values) {
                j[key] = value;
            }

            return j;
        }
    }
}
