#ifndef SDL03_Game_Objects_Characters_Base
#define SDL03_Game_Objects_Characters_Base

#include <algorithm>
#include <cmath>
#include <memory>
#include <random>
#include <string>

#include "../../../helpers/file_system.hpp"
#include "../base.hpp"
#include "../../parsers/asset_list_parser.hpp"
#include "../../resources/texture.hpp"

namespace Game {
    namespace Parsers {
        class CharacterParser;
    }

    namespace Objects {
        namespace Characters {
            class Base : public Objects::Base {
            public:
                std::string name;
                std::string spriteName;
                std::string spritesheetName;
                std::shared_ptr<Resources::Texture> sprite;
                std::shared_ptr<Resources::Texture> spritesheet;

                Base();
                ~Base();
                unsigned long long GetLevel();
                unsigned long long SetLevel(const unsigned long long newLevel);
                unsigned long long GetCurrentHitPoints();
                unsigned long long SetCurrentHitPoints(const unsigned long long newCurrentHitPoints);
                unsigned long long GetMaxHitPoints();
                unsigned long long SetMaxHitPoints(const unsigned long long newMaxHitPoints);
                unsigned long long GetCurrentMagicPoints();
                unsigned long long SetCurrentMagicPoints(const unsigned long long newCurrentMagicPoints);
                unsigned long long GetMaxMagicPoints();
                unsigned long long SetMaxMagicPoints(const unsigned long long newMaxMagicPoints);
                unsigned long long GetStrength();
                unsigned long long SetStrength(const unsigned long long newStrength);
                unsigned long long GetDexterity();
                unsigned long long SetDexterity(const unsigned long long newDexterity);
                unsigned long long GetIntelligence();
                unsigned long long SetIntelligence(const unsigned long long newIntelligence);
                unsigned long long GetVitality();
                unsigned long long SetVitality(const unsigned long long newVitality);
                unsigned long long GetStamina();
                unsigned long long SetStamina(const unsigned long long newStamina);
                unsigned long long GetLuck();
                unsigned long long SetLuck(const unsigned long long newLuck);
                unsigned long long Damage(const unsigned long long damage);
                int ATBStart();
                bool Load(const std::string& filename);
                bool Parse(const std::string& jsonString);
                void Render(const int x, const int y);
            protected:
                unsigned long long level;
                unsigned long long currentHitPoints;
                unsigned long long maxHitPoints;
                unsigned long long currentMagicPoints;
                unsigned long long maxMagicPoints;
                unsigned long long strength;
                unsigned long long dexterity;
                unsigned long long intelligence;
                unsigned long long vitality;
                unsigned long long stamina;
                unsigned long long luck;

                bool ParseCharacterFile(const std::string& jsonString);

                class Parser {
                public:
                    Parser();
                    ~Parser();
                    void Parse(const std::string& jsonString, std::shared_ptr<Base> character);

                private:
                    static const std::string logChannel;

                    std::shared_ptr<Log::Logger> logger;
                };

            public:
                class LuaInterface {
                public:
                    static void Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext);
                };
            };
        }
    }
}

#endif
