#ifndef SDL03_Game_World_Flags
#define SDL03_Game_World_Flags

#include <map>

#include "../../../lib/nlohmann/json.hpp"

using json = nlohmann::json;

namespace Game {
    namespace World {
        class Flags {
        public:
            Flags();
            ~Flags();

            bool Exists(const std::string& key) const;
            bool Get(const std::string& key, bool defaultValue = false) const;
            void Set(const std::string& key, bool value);

            json AsJSON() const;

        private:
            std::unordered_map<std::string, bool> values;
        };
    }
}

#endif
