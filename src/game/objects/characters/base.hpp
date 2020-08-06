#ifndef SDL03_Game_Objects_Characters_Base
#define SDL03_Game_Objects_Characters_Base

#include <algorithm>
#include <cmath>
#include <memory>
#include <random>
#include <string>

#include "../../../../lib/nlohmann/json.hpp"

#include "../../../helpers/file_system.hpp"
#include "../../resources/texture.hpp"
#include "../base.hpp"

using json = nlohmann::json;

namespace Game {
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
                std::string GetName();
                std::string SetName(const std::string& newName);
                unsigned long long int GetLevel();
                unsigned long long int SetLevel(const unsigned long long int newLevel);
                unsigned long long int GetCurrentHitPoints();
                unsigned long long int SetCurrentHitPoints(const unsigned long long int newCurrentHitPoints);
                unsigned long long int GetMaxHitPoints();
                unsigned long long int SetMaxHitPoints(const unsigned long long int newMaxHitPoints);
                unsigned long long int GetCurrentMagicPoints();
                unsigned long long int SetCurrentMagicPoints(const unsigned long long int newCurrentMagicPoints);
                unsigned long long int GetMaxMagicPoints();
                unsigned long long int SetMaxMagicPoints(const unsigned long long int newMaxMagicPoints);
                unsigned long long int GetStrength();
                unsigned long long int SetStrength(const unsigned long long int newStrength);
                unsigned long long int GetDexterity();
                unsigned long long int SetDexterity(const unsigned long long int newDexterity);
                unsigned long long int GetIntelligence();
                unsigned long long int SetIntelligence(const unsigned long long int newIntelligence);
                unsigned long long int GetVitality();
                unsigned long long int SetVitality(const unsigned long long int newVitality);
                unsigned long long int GetStamina();
                unsigned long long int SetStamina(const unsigned long long int newStamina);
                unsigned long long int GetLuck();
                unsigned long long int SetLuck(const unsigned long long int newLuck);
                unsigned long long int Damage(const unsigned long long int damage);
                unsigned long long int Damage(const unsigned long long int damage, const bool limited);
                unsigned long long int Heal(const unsigned long long int damage);
                unsigned long long int Heal(const unsigned long long int damage, const bool limited);
                unsigned int ATBStart();
                bool Load(const std::string& filename);
                bool Parse(const std::string& jsonString);
                void Render(const int x, const int y);

            protected:
                unsigned long long int level;
                unsigned long long int currentHitPoints;
                unsigned long long int maxHitPoints;
                unsigned long long int currentMagicPoints;
                unsigned long long int maxMagicPoints;
                unsigned long long int strength;
                unsigned long long int dexterity;
                unsigned long long int intelligence;
                unsigned long long int vitality;
                unsigned long long int stamina;
                unsigned long long int luck;

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
            };
        }
    }
}

#endif
