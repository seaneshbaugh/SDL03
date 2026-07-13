#ifndef SDL03_Game_Scripts_ScriptSelector
#define SDL03_Game_Scripts_ScriptSelector

#include "script_profile.hpp"
#include "../world/conditions/evaluation_contexts/evaluation_context.hpp"

namespace Game {
    namespace Scripts {
        class ScriptSelector {
        public:
            ScriptSelector(const std::string& scriptProfileId);
            ~ScriptSelector();

            std::string SelectScript(const World::Conditions::EvaluationContexts::EvaluationContext& context);

        private:
            ScriptProfile scriptProfile;
        };
    }
}

#endif
