#ifndef SDL03_Game_Actions_ActionParser
#define SDL03_Game_Actions_ActionParser

#include "../../../../lib/nlohmann/json.hpp"

#include "../services/locator.hpp"
#include "add_actor.hpp"
#include "animate_actor.hpp"
#include "face_actor.hpp"
#include "move_actor.hpp"
#include "parallel.hpp"
#include "pathfind_actor.hpp"
#include "remove_actor.hpp"
#include "sequence.hpp"
#include "set_flag.hpp"
#include "wait.hpp"

using json = nlohmann::json;

namespace Game {
    namespace Scripts {
        class Script;
    }

    namespace Actions {
        class ActionParser {
        public:
            ActionParser();
            ~ActionParser();
            std::shared_ptr<Actions::Base> ParseAction(const json& node, Scripts::Script* script);

        private:
            using ActionFactory = std::function<std::shared_ptr<Actions::Base>(const json& node, Scripts::Script* script)>;

            static const std::string logChannel;

            std::shared_ptr<Log::Logger> logger;
            std::unordered_map<std::string, ActionFactory> actionFactories;
        };
    }
}

#endif
