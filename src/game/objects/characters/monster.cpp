#include "monster.hpp"

namespace Game {
    namespace Objects {
        namespace Characters {
            Monster::Monster() : Base() {
            }

            Monster::~Monster() {
            }

            void Monster::Render(const float x, const float y) {
                //// TODO: Use actual width and height of the sprite instead of hardcoding 32x32.
                //// This is just a placeholder until I have actual sprites to work with.
                //const SDL_FRect srcrect = {0.0f, 0.0f, 32.0f, 32.0f};

                //const SDL_FRect dstrect = {x, y, 32.0f, 32.0f};

                //Services::Locator::VideoService()->RenderTexture(this->sprite, &srcrect, &dstrect);
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
