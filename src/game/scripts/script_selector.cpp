#include "script_selector.hpp"

namespace Game {
    namespace Scripts {
        ScriptSelector::ScriptSelector(const std::string& scriptProfileId) : scriptProfile(scriptProfileId) {
        }

        ScriptSelector::~ScriptSelector() {
        }

        std::string ScriptSelector::SelectScript(const World::Conditions::EvaluationContexts::EvaluationContext& context) {
            for (const auto& entry : this->scriptProfile.entries) {
                if (!entry.condition) {
                    return entry.scriptId;
                }

                if (entry.condition->Evaluate(context)) {
                    return entry.scriptId;
                }
            }

            // TODO: Make this return an optional?
            return "";
        }
    }
}
