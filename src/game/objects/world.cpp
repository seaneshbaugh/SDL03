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

        // TODO: Maybe make this be a static method for Objects::Characters::Party.
        void World::SetStartingPlayerParty() {
            this->UnloadPlayerParty();

            this->playerParty = std::make_shared<Objects::Characters::Party>();

            std::shared_ptr<Objects::Characters::Base> sean = std::make_shared<Objects::Characters::Base>();

            sean->Load("resources/characters/character01.json");

            this->playerParty->characters.push_back(sean);

            std::shared_ptr<Objects::Characters::Base> casie = std::make_shared<Objects::Characters::Base>();

            casie->Load("resources/characters/character02.json");

            this->playerParty->characters.push_back(casie);
        }

        void World::UnloadPlayerParty() {
            this->playerParty.reset();
        }

        void World::SetEnemyParty(std::shared_ptr<Objects::Maps::MapEncounterArea> encounterArea) {
            this->UnloadEnemyParty();

            this->enemyParty = std::make_shared<Objects::Characters::Party>();

            // TODO: Consider making rd and mt be members of World.
            std::random_device rd;

            std::mt19937_64 mt(rd());

            const int ml = static_cast<int>(encounterArea->mobs.size()) - 1;

            std::uniform_int_distribution<int> dist(0, ml);

            int mobIndex = dist(mt);

            std::vector<std::string> mob = encounterArea->mobs[mobIndex];

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
    }
}
