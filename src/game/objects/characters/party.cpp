#include "party.hpp"

namespace Game {
    namespace Objects {
        namespace Characters {
            Party::Party() {
            }

            Party::~Party() {
            }

            std::shared_ptr<Base> Party::GetLeader() {
                return this->leader;
            }

            // Not so sure how I feel about this using an index. For now though that'll
            // make it easier to work with from Lua.
            std::shared_ptr<Base> Party::SetLeader(const int index) {
                this->leader = this->characters[index];

                return this->leader;
            }

            void Party::LuaInterface::Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext) {

            }
        }
    }
}
