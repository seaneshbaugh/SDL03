#include "base.hpp"

namespace Game {
    namespace Objects {
        namespace Characters {
            const unsigned long long int MIN_LEVEL = 1;
            const unsigned long long int MAX_LEVEL = 99;
            const unsigned long long int MIN_HIT_POINTS = 0;
            const unsigned long long int MIN_MAX_HIT_POINTS = 1;
            const unsigned long long int MAX_MAX_HIT_POINTS = 999999999999999;
            const unsigned long long int MIN_MAGIC_POINTS = 0;
            const unsigned long long int MIN_MAX_MAGIC_POINTS = 0;
            const unsigned long long int MAX_MAX_MAGIC_POINTS = 999999999999999;
            const unsigned long long int MIN_STAT = 1;
            const unsigned long long int MAX_STAT = 9999;
            const unsigned long long int MIN_DAMAGE = 0;
            const unsigned long long int MAX_LIMITED_DAMAGE = 9999;
            const unsigned long long int MAX_UNLIMITED_DAMAGE = 999999999999999;

            Base::Base() {
                this->name = "";
                this->level = MIN_LEVEL;
                this->currentHitPoints = MIN_MAX_HIT_POINTS;
                this->maxHitPoints = MIN_MAX_HIT_POINTS;
                this->currentMagicPoints = MIN_MAX_MAGIC_POINTS;
                this->maxMagicPoints = MIN_MAX_MAGIC_POINTS;
                this->strength = MIN_STAT;
                this->dexterity = MIN_STAT;
                this->intelligence = MIN_STAT;
                this->vitality = MIN_STAT;
                this->stamina = MIN_STAT;
                this->luck = MIN_STAT;
                this->spriteName = "";
                this->spritesheetName = "";
            }

            Base::~Base() {
            }

            std::string Base::GetName() {
                return this->name;
            }

            std::string Base::SetName(const std::string& newName) {
                this->name = newName;

                return this->name;
            }

            unsigned long long int Base::GetLevel() {
                return this->level;
            }

            unsigned long long int Base::SetLevel(const unsigned long long int newLevel) {
                this->level = std::clamp(newLevel, MIN_LEVEL, MAX_LEVEL);

                return this->level;
            }

            unsigned long long int Base::GetCurrentHitPoints() {
                return this->currentHitPoints;
            }

            unsigned long long int Base::SetCurrentHitPoints(const unsigned long long int newCurrentHitPoints) {
                this->currentHitPoints = std::clamp(newCurrentHitPoints, MIN_HIT_POINTS, this->maxHitPoints);

                return this->currentHitPoints;
            }

            unsigned long long int Base::GetMaxHitPoints() {
                return this->maxHitPoints;
            }

            unsigned long long int Base::SetMaxHitPoints(const unsigned long long int newMaxHitPoints) {
                this->maxHitPoints = std::clamp(newMaxHitPoints, MIN_MAX_HIT_POINTS, MAX_MAX_HIT_POINTS);

                if (this->currentHitPoints > this->maxHitPoints) {
                    this->currentHitPoints = this->maxHitPoints;
                }

                return this->maxHitPoints;
            }

            unsigned long long int Base::GetCurrentMagicPoints() {
                return this->currentMagicPoints;
            }

            unsigned long long int Base::SetCurrentMagicPoints(const unsigned long long int newCurrentMagicPoints) {
                this->currentMagicPoints = std::clamp(newCurrentMagicPoints, MIN_MAGIC_POINTS, this->maxMagicPoints);

                return this->currentMagicPoints;
            }

            unsigned long long int Base::GetMaxMagicPoints() {
                return this->maxMagicPoints;
            }

            unsigned long long int Base::SetMaxMagicPoints(const unsigned long long int newMaxMagicPoints) {
                this->maxMagicPoints = std::clamp(newMaxMagicPoints, MIN_MAX_MAGIC_POINTS, MAX_MAX_MAGIC_POINTS);

                if (this->currentMagicPoints > this->maxMagicPoints) {
                    this->currentMagicPoints = this->maxMagicPoints;
                }

                return this->maxMagicPoints;
            }

            unsigned long long int Base::GetStrength() {
                return this->strength;
            }

            unsigned long long int Base::SetStrength(const unsigned long long int newStrength) {
                this->strength = std::clamp(newStrength, MIN_STAT, MAX_STAT);

                return this->strength;
            }

            unsigned long long int Base::GetDexterity() {
                return this->dexterity;
            }

            unsigned long long int Base::SetDexterity(const unsigned long long int newDexterity) {
                this->dexterity = std::clamp(newDexterity, MIN_STAT, MAX_STAT);

                return this->strength;
            }

            unsigned long long int Base::GetIntelligence() {
                return this->intelligence;
            }

            unsigned long long int Base::SetIntelligence(const unsigned long long int newIntelligence) {
                this->intelligence = std::clamp(newIntelligence, MIN_STAT, MAX_STAT);

                return this->intelligence;
            }

            unsigned long long int Base::GetVitality() {
                return this->vitality;
            }

            unsigned long long int Base::SetVitality(const unsigned long long int newVitality) {
                this->vitality = std::clamp(newVitality, MIN_STAT, MAX_STAT);

                return this->vitality;
            }

            unsigned long long int Base::GetStamina() {
                return this->stamina;
            }

            unsigned long long int Base::SetStamina(const unsigned long long int newStamina) {
                this->stamina = std::clamp(newStamina, MIN_STAT, MAX_STAT);

                return this->stamina;
            }

            unsigned long long Base::GetLuck() {
                return this->luck;
            }

            unsigned long long int Base::SetLuck(const unsigned long long int newLuck) {
                this->luck = std::clamp(newLuck, MIN_STAT, MAX_STAT);

                return this->luck;
            }

            unsigned long long int Base::Damage(const unsigned long long int damage) {
                return this->Damage(damage, true);
            }

            unsigned long long int Base::Damage(const unsigned long long int damage, const bool limited) {
                const unsigned long long int maxDamage = limited ? MAX_LIMITED_DAMAGE : MAX_UNLIMITED_DAMAGE;

                const unsigned long long int damageDealt = std::clamp(damage, MIN_DAMAGE, maxDamage);

                unsigned long long int newCurrentHitPoints = this->currentHitPoints - damageDealt;

                if (newCurrentHitPoints > this->currentHitPoints) {
                    newCurrentHitPoints = MIN_HIT_POINTS;
                }

                this->currentHitPoints = newCurrentHitPoints;

                return damageDealt;
            }

            unsigned long long int Base::Heal(const unsigned long long int damage) {
                return this->Heal(damage, true);
            }

            unsigned long long int Base::Heal(const unsigned long long int damage, const bool limited) {
                const unsigned long long int maxDamage = limited ? MAX_LIMITED_DAMAGE : MAX_UNLIMITED_DAMAGE;

                const unsigned long long int damageHealed = std::clamp(damage, MIN_DAMAGE, maxDamage);

                unsigned long long int newCurrentHitPoints = this->currentHitPoints + damageHealed;

                if (newCurrentHitPoints < this->currentHitPoints) {
                    newCurrentHitPoints = this->maxHitPoints;
                }

                this->currentHitPoints = newCurrentHitPoints;

                return damageHealed;
            }

            // I would have put this in the Lua script but I don't know enough about how it does
            // arithmetic (it's all floating point?) and even less about how it handles random
            // numbers to trust it. I may end up making a collection of functions that handle
            // all of the random number generation I will need and have those functions make
            // calls to std::uniform_int_distribution and std::uniform_real_distribution so I
            // can keep logic like this out of the C++ code.
            // As a character's dexterity approaches 255 (which is well below the maximum, but
            // still very high as I envision most level 99 characters topping out in the 110 to
            // 140 range) their ATB start approaches a minimum value of 100 (i.e. ready to take
            // action immediately, as if they had a preemptive attack). In addition to the
            // minimum value they also get a bonus equal to a random number between 0 and their
            // dexterity over 255. In other words, at 128 dexterity the character will start
            // with at least 50  but possibly up to 100. As their minimum start value increases
            // so does their possible maximum start value. A start value over 100 makes no sense
            // so no matter how dexterous the character is their start value is limited to 100.
            unsigned int Base::ATBStart() {
                double dexterityRatio = (static_cast<double>(this->dexterity) / 255.0) * 100.0;
                std::random_device rd;
                std::mt19937 mt(rd());
                std::uniform_real_distribution<double> dist(0.0, dexterityRatio);
                double atbStart = std::floor(dexterityRatio + dist(mt));

                if (atbStart > 100.0) {
                    atbStart = 100.0;
                }

                return static_cast<unsigned int>(atbStart);
            }

            bool Base::ParseCharacterFile(const std::string& jsonString) {
                std::unique_ptr<Base::Parser> parser = std::make_unique<Base::Parser>();

                parser->Parse(jsonString, std::static_pointer_cast<Base>(this->shared_from_this()));

                return true;
            }

            bool Base::Load(const std::string& filename) {
                std::string jsonString;

                if (!Helpers::FileSystem::ReadFile(filename, jsonString)) {
                    return false;
                }

                if (!this->ParseCharacterFile(jsonString)) {
                    return false;
                }

                return true;
            }

            void Base::Render(const int x, const int y) {
                const SDL_Rect srcrect = {32, 32, 32, 32};

                const SDL_Rect dstrect = {x, y, 32, 32};

                Services::Locator::VideoService()->Render(this->sprite, &srcrect, &dstrect);
            }

            const std::string Base::Parser::logChannel = "json";

            Base::Parser::Parser() {
                this->logger = Services::Locator::LoggerService()->GetLogger(Base::Parser::logChannel);
            }

            Base::Parser::~Parser() {
            }

            void Base::Parser::Parse(const std::string& jsonString, std::shared_ptr<Base> character) {
                json characterNode = json::parse(jsonString);

                character->name = characterNode["name"].get<std::string>();
                character->SetLevel(characterNode["level"].get<unsigned long long int>());
                const unsigned long long int hitPoints = characterNode["hitPoints"].get<unsigned long long int>();
                character->SetMaxHitPoints(hitPoints);
                character->SetCurrentHitPoints(hitPoints);
                const unsigned long long int magicPoints = characterNode["magicPoints"].get<unsigned long long int>();
                character->SetMaxMagicPoints(magicPoints);
                character->SetCurrentMagicPoints(magicPoints);
                character->SetStrength(characterNode["strength"].get<unsigned long long int>());
                character->SetDexterity(characterNode["dexterity"].get<unsigned long long int>());
                character->SetIntelligence(characterNode["intelligence"].get<unsigned long long int>());
                character->SetVitality(characterNode["vitality"].get<unsigned long long int>());
                character->SetStamina(characterNode["stamina"].get<unsigned long long int>());
                character->SetLuck(characterNode["luck"].get<unsigned long long int>());
                character->spriteName = "characters.sprite." + character->name;
                character->spritesheetName = "characters.spritesheet." + character->name;
                character->sprite = Services::Locator::TextureService()->AddTexture(character->spriteName, characterNode["sprite"].get<std::string>());
                character->spritesheet = Services::Locator::TextureService()->AddTexture(character->spritesheetName, characterNode["spritesheet"].get<std::string>());
            }

            void Base::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
                sol::table objects = (*luaState.get())["objects"].get_or_create<sol::table>(sol::new_table());

                sol::table characters = objects["characters"].get_or_create<sol::table>(sol::new_table());

                characters.new_usertype<Base>("Base",
                                              sol::constructors<Base()>(),
                                              "getName", &Base::GetName,
                                              "setName", &Base::SetName,
                                              "getLevel", &Base::GetLevel,
                                              "setLevel", &Base::SetLevel,
                                              "getCurrentHitPoints", &Base::GetCurrentHitPoints,
                                              "setCurrentHitPoints", &Base::SetCurrentHitPoints,
                                              "getMaxHitPoints", &Base::GetMaxHitPoints,
                                              "setMaxHitPoints", &Base::SetMaxHitPoints,
                                              "getCurrentMagicPoints", &Base::GetCurrentMagicPoints,
                                              "setCurrentMagicPoints", &Base::SetCurrentMagicPoints,
                                              "getMaxMagicPoints", &Base::GetMaxMagicPoints,
                                              "setMaxMagicPoints", &Base::SetMaxMagicPoints,
                                              "getStrength", &Base::GetStrength,
                                              "getDexterity", &Base::GetDexterity,
                                              "getIntelligence", &Base::GetIntelligence,
                                              "atbStart", &Base::ATBStart,
                                              "damage", sol::overload(
                                                                      static_cast<unsigned long long int (Base::*)(const unsigned long long int)>(&Base::Damage),
                                                                      static_cast<unsigned long long int (Base::*)(const unsigned long long int, const bool)>(&Base::Damage)
                                                                      ),
                                              "heal", sol::overload(
                                                                    static_cast<unsigned long long int (Base::*)(const unsigned long long int)>(&Base::Heal),
                                                                    static_cast<unsigned long long int (Base::*)(const unsigned long long int, const bool)>(&Base::Heal)
                                                                      ),
                                              "render", &Base::Render
                                              );
            }
        }
    }
}
