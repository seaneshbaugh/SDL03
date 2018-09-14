#include "CharacterParser.h"
#include "GameCharacter.h"

CharacterParser::CharacterParser() {
    this->logger = new Log::Logger("json");
}

CharacterParser::~CharacterParser() {
    if (this->logger != nullptr) {
        delete this->logger;
    }
}

bool CharacterParser::Parse(std::string jsonString, GameCharacter* character) {
    JSONNode characterNode = libjson::parse(jsonString);

    character->name = characterNode.at("name").as_string();
    character->SetLevel(static_cast<int>(characterNode.at("level").as_int()));
    const unsigned long long hitPoints = static_cast<unsigned long long>(characterNode.at("hitPoints").as_int());
    character->SetMaxHitPoints(hitPoints);
    character->SetCurrentHitPoints(hitPoints);
    const unsigned long long magicPoints = static_cast<unsigned long long>(characterNode.at("magicPoints").as_int());
    character->SetMaxMagicPoints(magicPoints);
    character->SetCurrentMagicPoints(magicPoints);
    character->SetStrength(static_cast<unsigned long long>(characterNode.at("strength").as_int()));
    character->SetDexterity(static_cast<unsigned long long>(characterNode.at("dexterity").as_int()));
    character->SetIntelligence(static_cast<unsigned long long>(characterNode.at("intelligence").as_int()));
    character->SetVitality(static_cast<unsigned long long>(characterNode.at("vitality").as_int()));
    character->SetStamina(static_cast<unsigned long long>(characterNode.at("stamina").as_int()));
    character->SetLuck(static_cast<unsigned long long>(characterNode.at("luck").as_int()));
    // This kinda feels out of place...
    character->sprite = std::make_shared<GameTexture>(characterNode.at("sprite").as_string());
    character->spritesheet = std::make_shared<GameTexture>(characterNode.at("spritesheet").as_string());
    character->spriteName = "characters.sprite." + character->name;
    character->spritesheetName = "characters.spritesheet." + character->name;
    Services::Locator::TextureService()->AddTexture(character->sprite, character->spriteName, nullptr);
    Services::Locator::TextureService()->AddTexture(character->spritesheet, character->spritesheetName, nullptr);

    return true;
}
