#include "monster.hpp"

namespace Game {
    namespace Objects {
        namespace Characters {
            Monster::Monster() : Base() {
            }

            Monster::~Monster() {
            }

            void Monster::Render(const int x, const int y) {
                const SDL_Rect srcrect = {0, 0, 32, 32};

                const SDL_Rect dstrect = {x, y, 32, 32};

                Services::Locator::VideoService()->Render(this->sprite, &srcrect, &dstrect);
            }

            void Monster::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
                sol::table objects = (*luaState.get())["objects"].get_or_create<sol::table>(sol::new_table());

                sol::table characters = objects["characters"].get_or_create<sol::table>(sol::new_table());

                characters.new_usertype<Monster>("Monster",
                                                 sol::constructors<Monster()>(),
                                                 "getName", &Monster::GetName,
                                                 "setName", &Monster::SetName,
                                                 "getLevel", &Monster::GetLevel,
                                                 "setLevel", &Monster::SetLevel,
                                                 "getCurrentHitPoints", &Monster::GetCurrentHitPoints,
                                                 "setCurrentHitPoints", &Monster::SetCurrentHitPoints,
                                                 "getMaxHitPoints", &Monster::GetMaxHitPoints,
                                                 "setMaxHitPoints", &Monster::SetMaxHitPoints,
                                                 "getCurrentMagicPoints", &Monster::GetCurrentMagicPoints,
                                                 "setCurrentMagicPoints", &Monster::SetCurrentMagicPoints,
                                                 "getMaxMagicPoints", &Monster::GetMaxMagicPoints,
                                                 "setMaxMagicPoints", &Monster::SetMaxMagicPoints,
                                                 "getStrength", &Monster::GetStrength,
                                                 "getDexterity", &Monster::GetDexterity,
                                                 "getIntelligence", &Monster::GetIntelligence,
                                                 "atbStart", &Monster::ATBStart,
                                                 "damage", sol::overload(
                                                                         static_cast<unsigned long long int (Monster::*)(const unsigned long long int)>(&Monster::Damage),
                                                                         static_cast<unsigned long long int (Monster::*)(const unsigned long long int, const bool)>(&Monster::Damage)
                                                                         ),
                                                 "heal", sol::overload(
                                                                       static_cast<unsigned long long int (Monster::*)(const unsigned long long int)>(&Monster::Heal),
                                                                       static_cast<unsigned long long int (Monster::*)(const unsigned long long int, const bool)>(&Monster::Heal)
                                                                       ),
                                                 "render", &Monster::Render,
                                                 sol::base_classes, sol::bases<Base>()
                                                 );
            }
        }
    }
}
