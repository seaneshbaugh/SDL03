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

    JSONNode::const_iterator i = characterNode.begin();

    // What I wouldn't give for Erlang's pattern matching right now...
    while (i != characterNode.end()) {
        if (i->name() == "name" && i->type() == JSON_STRING) {
            character->name = i->as_string();
        } else {
            if (i->name() == "level" && i->type() == JSON_NUMBER) {
                character->SetLevel(static_cast<unsigned long long>(i->as_int()));
            } else {
                if (i->name() == "hitPoints" && i->type() == JSON_NUMBER) {
                    character->SetMaxHitPoints(static_cast<unsigned long long>(i->as_int()));

                    character->SetCurrentHitPoints(static_cast<unsigned long long>(i->as_int()));
                } else {
                    if (i->name() == "magicPoints" && i->type() == JSON_NUMBER) {
                        character->SetMaxMagicPoints(static_cast<unsigned long long>(i->as_int()));

                        character->SetCurrentMagicPoints(static_cast<unsigned long long>(i->as_int()));
                    } else {
                        if (i->name() == "strength" && i->type() == JSON_NUMBER) {
                            character->SetStrength(static_cast<unsigned long long>(i->as_int()));
                        } else {
                            if (i->name() == "dexterity" && i->type() == JSON_NUMBER) {
                                character->SetDexterity(static_cast<unsigned long long>(i->as_int()));
                            } else {
                                if (i->name() == "intelligence" && i->type() == JSON_NUMBER) {
                                    character->SetIntelligence(static_cast<unsigned long long>(i->as_int()));
                                } else {
                                    if (i->name() == "vitality" && i->type() == JSON_NUMBER) {
                                        character->SetVitality(static_cast<unsigned long long>(i->as_int()));
                                    } else {
                                        if (i->name() == "stamina" && i->type() == JSON_NUMBER) {
                                            character->SetStamina(static_cast<unsigned long long>(i->as_int()));
                                        } else {
                                            if (i->name() == "luck" && i->type() == JSON_NUMBER) {
                                                character->SetLuck(static_cast<unsigned long long>(i->as_int()));
                                            } else {
                                                if (i->name() == "sprite" && i->type() == JSON_STRING) {
                                                    character->sprite = new GameTexture(i->as_string());
                                                } else {
                                                    if (i->name() == "spritesheet" && i->type() == JSON_STRING) {
                                                        character->spritesheet = new GameTexture(i->as_string());
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        i++;
    }

    return true;
}
