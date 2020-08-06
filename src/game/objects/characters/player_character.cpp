#include "player_character.hpp"

namespace Game {
    namespace Objects {
        namespace Characters {
            // Values generated by the equation ceil(450.0 * e^0.1739x) - 460.0. See
            // utils/level-curve for more details.
            const unsigned long long int PlayerCharacter::EXPERIENCE_REQUIRED_FOR_LEVEL[99] = {
                0, 80, 180, 300, 450, 620, 820, 1070, 1350, 1700,
                2110, 2590, 3170, 3860, 4680, 5660, 6820, 8200, 9840, 11800,
                14120, 16890, 20190, 24110, 28770, 34320, 40930, 48790, 58140, 69270,
                82520, 98280, 117030, 139340, 165890, 197490, 235090, 279830, 333060, 396410,
                471790, 561480, 668220, 795220, 946350, 1126180, 1340170, 1594810, 1897800, 2258350,
                2687380, 3197900, 3805390, 4528250, 5388420, 6411970, 7629920, 9079210, 10803770, 12855880,
                15297770, 18203470, 21661060, 25775370, 30671140, 36496800, 43428960, 51677800, 61493390, 73173320,
                87071690, 103609870, 123289260, 146706490, 174571500, 207729100, 247184540, 294134010, 350000900, 416478960,
                495583620, 589713160, 701721340, 835003970, 993601840, 1182323250, 1406889760, 1674109650, 1992084380, 2370454130,
                2820690140, 3356442420, 3993953660, 4752551590, 5655235000, 6729371010, 8007524720, 9528446550, 11338247060
            };

            void PlayerCharacter::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
                sol::table objects = (*luaState.get())["objects"].get_or_create<sol::table>(sol::new_table());

                sol::table characters = objects["characters"].get_or_create<sol::table>(sol::new_table());

                characters.new_usertype<PlayerCharacter>("PlayerCharacter",
                                                         sol::constructors<PlayerCharacter()>(),
                                                         "getName", &PlayerCharacter::GetName,
                                                         "setName", &PlayerCharacter::SetName,
                                                         "getLevel", &PlayerCharacter::GetLevel,
                                                         "setLevel", &PlayerCharacter::SetLevel,
                                                         "getCurrentHitPoints", &PlayerCharacter::GetCurrentHitPoints,
                                                         "setCurrentHitPoints", &PlayerCharacter::SetCurrentHitPoints,
                                                         "getMaxHitPoints", &PlayerCharacter::GetMaxHitPoints,
                                                         "setMaxHitPoints", &PlayerCharacter::SetMaxHitPoints,
                                                         "getCurrentMagicPoints", &PlayerCharacter::GetCurrentMagicPoints,
                                                         "setCurrentMagicPoints", &PlayerCharacter::SetCurrentMagicPoints,
                                                         "getMaxMagicPoints", &PlayerCharacter::GetMaxMagicPoints,
                                                         "setMaxMagicPoints", &PlayerCharacter::SetMaxMagicPoints,
                                                         "getStrength", &PlayerCharacter::GetStrength,
                                                         "getDexterity", &PlayerCharacter::GetDexterity,
                                                         "getIntelligence", &PlayerCharacter::GetIntelligence,
                                                         "atbStart", &PlayerCharacter::ATBStart,
                                                         "damage", sol::overload(
                                                                                 static_cast<unsigned long long int (PlayerCharacter::*)(const unsigned long long int)>(&PlayerCharacter::Damage),
                                                                                 static_cast<unsigned long long int (PlayerCharacter::*)(const unsigned long long int, const bool)>(&PlayerCharacter::Damage)
                                                                      ),
                                                         "heal", sol::overload(
                                                                               static_cast<unsigned long long int (PlayerCharacter::*)(const unsigned long long int)>(&PlayerCharacter::Heal),
                                                                               static_cast<unsigned long long int (PlayerCharacter::*)(const unsigned long long int, const bool)>(&PlayerCharacter::Heal)
                                                                    ),
                                                         "render", &PlayerCharacter::Render
                                                         );
            }
        }
    }
}
