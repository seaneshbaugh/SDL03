#include "dialogue_profile.hpp"

namespace Game {
    namespace Scenes {
        namespace Dialogue {
            DialogueProfile::DialogueProfile(const std::string& profileName) {
                this->profileName = profileName;
                this->Load();
            }

            DialogueProfile::~DialogueProfile() {
            }

            bool DialogueProfile::Load() {
                DialogueProfile::Parser parser = DialogueProfile::Parser();

                std::string jsonString;

                if (!Helpers::FileSystem::ReadFile("data/dialogue/profiles/" + this->profileName + ".json", jsonString)) {
                    return false;
                }

                return parser.Parse(jsonString, this);
            }

            DialogueProfile::Parser::Parser() {
            }

            DialogueProfile::Parser::~Parser() {
            }

            bool DialogueProfile::Parser::Parse(const std::string& jsonString, DialogueProfile* dialogueProfile) {
                json profileNode = json::parse(jsonString);

                if (profileNode.find("dialogues") == profileNode.end()) {
                    return false;
                }

                json dialoguesNode = profileNode["dialogues"];

                for (auto it = dialoguesNode.begin(); it != dialoguesNode.end(); ++it) {
                    if (it->is_object()) {
                        DialogueProfile::Entry entry;

                        if (it->find("condition") != it->end()) {
                            entry.condition = std::make_unique<World::Conditions::Condition>((*it)["condition"].get<std::string>());
                        }

                        entry.dialogueId = (*it)["dialogueId"].get<std::string>();

                        dialogueProfile->entries.push_back(std::move(entry));
                    }
                }

                return true;
            }
        }
    }
}
