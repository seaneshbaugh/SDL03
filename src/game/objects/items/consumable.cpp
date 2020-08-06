#include "consumable.hpp"

namespace Game {
    namespace Objects {
        namespace Items {
            void Consumable::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
                sol::table objects = (*luaState.get())["objects"].get_or_create<sol::table>(sol::new_table());

                sol::table items = objects["items"].get_or_create<sol::table>(sol::new_table());

                items.new_usertype<Consumable>("Consumable",
                                               sol::constructors<Consumable()>(),
                                               "getId", &Consumable::GetName,
                                               "setId", &Consumable::SetName,
                                               "getName", &Consumable::GetName,
                                               "setName", &Consumable::SetName
                                               );
            }
        }
    }
}
