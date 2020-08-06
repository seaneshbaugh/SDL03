#include "equippable.hpp"

namespace Game {
    namespace Objects {
        namespace Items {
            void Equippable::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
                sol::table objects = (*luaState.get())["objects"].get_or_create<sol::table>(sol::new_table());

                sol::table items = objects["items"].get_or_create<sol::table>(sol::new_table());

                items.new_usertype<Equippable>("Equippable",
                                               sol::constructors<Equippable()>(),
                                               "getId", &Equippable::GetName,
                                               "setId", &Equippable::SetName,
                                               "getName", &Equippable::GetName,
                                               "setName", &Equippable::SetName
                                               );
            }
        }
    }
}
