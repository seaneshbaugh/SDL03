#include "script_profile.hpp"

namespace Game {
    namespace Scripts {
        ScriptProfile::ScriptProfile(const std::string& profileName) {
            this->profileName = profileName;
            this->Load();
        }

        ScriptProfile::~ScriptProfile() {
        }

        bool ScriptProfile::Load() {
            ScriptProfile::Parser parser = ScriptProfile::Parser();

            std::string jsonString;

            if (!Helpers::FileSystem::ReadFile("data/scripts/profiles/" + this->profileName + ".json", jsonString)) {
                return false;
            }

            return parser.Parse(jsonString, this);
        }

        ScriptProfile::Parser::Parser() {
        }

        ScriptProfile::Parser::~Parser() {
        }

        bool ScriptProfile::Parser::Parse(const std::string& jsonString, ScriptProfile* scriptProfile) {
            json profileNode = json::parse(jsonString);

            if (profileNode.find("scripts") == profileNode.end()) {
                return false;
            }

            json scriptsNode = profileNode["scripts"];

            for (auto it = scriptsNode.begin(); it != scriptsNode.end(); ++it) {
                if (it->is_object()) {
                    ScriptProfile::Entry entry;

                    if (it->find("condition") != it->end()) {
                        entry.condition = std::make_unique<World::Conditions::Condition>((*it)["condition"].get<std::string>());
                    }

                    entry.scriptId = (*it)["scriptId"].get<std::string>();

                    scriptProfile->entries.push_back(std::move(entry));
                }
            }

            return true;
        }
    }
}
