#include "world.hpp"

namespace Game {
    namespace Objects {
        const std::string World::defaultMap = "resources/maps/world01.json";

        World::World() {
            this->playerParty = nullptr;
            this->SetStartingPlayerParty();
            this->enemyParty = nullptr;
            this->currentMap = nullptr;
            this->LoadMap(World::defaultMap);
        }

        World::~World() {
            this->UnloadPlayerParty();
            this->UnloadEnemyParty();
            this->UnloadMap();
        }

        void World::SetStartingPlayerParty() {
            this->UnloadPlayerParty();

            this->playerParty = std::make_shared<Objects::Characters::Party>();

            std::shared_ptr<Objects::Characters::PlayerCharacter> sean = std::make_shared<Objects::Characters::PlayerCharacter>();

            sean->Load("resources/characters/character01.json");

            this->playerParty->characters.push_back(sean);

            std::shared_ptr<Objects::Characters::PlayerCharacter> casie = std::make_shared<Objects::Characters::PlayerCharacter>();

            casie->Load("resources/characters/character02.json");

            this->playerParty->characters.push_back(casie);

            this->playerParty->SetLeader(0);

            // TODO: Move this elsewhere...
            std::shared_ptr<Objects::Items::Consumable> potion = std::static_pointer_cast<Objects::Items::Consumable>(Services::Locator::ItemService()->GetItem("lesser_health_potion"));

            // Something something law of demeter.
            this->playerParty->GetInventory()->AddItem(potion);
            this->playerParty->GetInventory()->AddItem(potion);
            this->playerParty->GetInventory()->AddItem(potion);
        }

        void World::UnloadPlayerParty() {
            this->playerParty.reset();
        }

        void World::SetEnemyParty(const Objects::Maps::MapEncounterArea& encounterArea) {
            this->UnloadEnemyParty();

            this->enemyParty = std::make_shared<Objects::Characters::Party>();

            // TODO: Consider making rd and mt be members of World.
            std::random_device rd;

            std::mt19937_64 mt(rd());

            const int ml = static_cast<int>(encounterArea.mobs.size()) - 1;

            std::uniform_int_distribution<int> dist(0, ml);

            int mobIndex = dist(mt);

            std::vector<std::string> mob = encounterArea.mobs[mobIndex];

            for (auto it = mob.begin(); it != mob.end(); it++) {
                std::shared_ptr<Objects::Characters::Monster> monster = std::make_shared<Objects::Characters::Monster>();

                // TODO: THIS IS BAD. Figure out a better way. The best solution may be some sort of
                // JSON resource file to act as an index of all monsters where the key is some sort
                // of ID and the value is the file to load it from. While I'm thinking about it,
                // the monster resource files should also include a reference to an AI (LOL) script
                // that the monster will use for determining its actions.
                if (*it == "Slime") {
                    monster->Load("resources/monsters/slime.json");
                } else {
                    // For now raise an exception since the above needs to be done and there are no
                    // other monster types defined yet anyways. If we get here something has gone
                    // wrong.
                    throw;
                }

                this->enemyParty->characters.push_back(monster);
            }
        }

        void World::UnloadEnemyParty() {
            this->enemyParty.reset();
        }

        void World::LoadMap(const std::string& filename) {
            this->UnloadMap();

            this->currentMap = std::make_shared<Objects::Maps::Map>(filename);
        }

        void World::UnloadMap() {
            this->currentMap.reset();
        }

        // TODO: Consider moving this to its own class. Every serializable object should
        // probably have its own serializer which is exposed through a Serialize method.
        // For now I'm using JSON for serializing stuff since I want to be able to easily
        // inspect save files. Eventually this may change to a more compact binary format
        // or JSON that's gzipped or something.
        json World::AsJSON() {
            json worldAsJSON;

            worldAsJSON["currentMap"] = this->currentMap->filename;
            worldAsJSON["playerCurrentX"] = this->playerCurrentX;
            worldAsJSON["playerCurrentY"] = this->playerCurrentY;
            worldAsJSON["playerParty"] = json::array();

            for (auto it = this->playerParty->characters.begin(); it != this->playerParty->characters.end(); ++it) {
                json characterAsJSON;

                // TODO: Serialize more stuff here. Or better yet do it in a character
                // serializer class.
                characterAsJSON["name"] = (*it)->GetName();
                characterAsJSON["level"] = (*it)->GetLevel();
                characterAsJSON["currentHitPoints"] = (*it)->GetCurrentHitPoints();
                characterAsJSON["maxHitPoints"] = (*it)->GetMaxHitPoints();

                worldAsJSON["playerParty"].push_back(characterAsJSON);
            }

            return worldAsJSON;
        }
    }
}
