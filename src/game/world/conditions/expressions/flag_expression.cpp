#include "flag_expression.hpp"
#include "../../../services/locator.hpp"
#include "../../state.hpp"

namespace Game {
    namespace World {
        namespace Conditions {
            namespace Expressions {
                 FlagExpression::FlagExpression(const std::string& key) : key(key) {
                 }

                 bool FlagExpression::Evaluate() {
                     return Services::Locator::WorldService()->GetState()->flags.Get(this->key, false);
                 }

            }
        }
    }
}
