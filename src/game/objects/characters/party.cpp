#include "party.hpp"

namespace Game {
    namespace Objects {
        namespace Characters {
            Party::Party() {
            }

            Party::~Party() {
            }

            std::shared_ptr<Characters::Base> Party::GetLeader() {
                return this->leader;
            }

            std::shared_ptr<Characters::Base> Party::SetLeader(const int index) {
                // TODO: Make sure index is in bounds.
                this->leader = this->characters[index];

                return this->leader;
            }

            void Party::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
                sol::table objects = (*luaState.get())["objects"].get_or_create<sol::table>(sol::new_table());

                sol::table characters = objects["characters"].get_or_create<sol::table>(sol::new_table());

                characters.new_usertype<Party>("Party",
                                               sol::no_constructor,
                                               "setLeader", &Party::SetLeader
                                               );
            }
        }
    }
}
