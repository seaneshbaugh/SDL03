#include "battle.hpp"

namespace Game {
    namespace States {
        const std::string Battle::logChannel = "states.battle";

        Battle::Battle(const Objects::Maps::MapEncounterArea* encounterArea) {
            this->logger = Services::Locator::LoggerService()->GetLogger(Battle::logChannel);
            this->pop = false;
            this->acceptRawInput = false;
            // This loads all background textures (well, just one for now, but if there were more
            // they would be loaded). In the future they should be removed from the texture list
            // and loaded only based on what's in the encounter area.
            this->LoadResources("resources/asset_lists/battle_textures.json", "resources/asset_lists/battle_sounds.json");

            this->backgroundName = "battle.background";

            this->background = Services::Locator::TextureService()->AddTexture(this->backgroundName, encounterArea->GetProperty("background"));

            Services::Locator::WorldService()->GetWorld()->SetEnemyParty(*encounterArea);

            this->LoadLuaState("scripts/states/battle.lua");
        }

        Battle::~Battle() {
            // TODO: CLEAR MONSTERS FROM WORLD VIA WORLD SERVICE!!!!!!
        }

        std::shared_ptr<Base> Battle::Update(const int key) {
            if (static_cast<InputKey>(key) != InputKey::NO_KEY) {
                this->ProcessInput(key);
            }

            std::string nextState = (*this->luaState.get())["update"]();

            if (this->pop) {
                return nullptr;
            }

            return this->shared_from_this();
        }

        std::shared_ptr<Base> Battle::Update(const SDL_Event& event) {
            return this->Update(event.key.keysym.sym);
        }

        std::string Battle::ProcessInput(const int key) {
            std::string result = (*this->luaState.get())["process_input"](key);

            return result;
        }

        void Battle::Render() {
            (*this->luaState.get())["render"]();
        }

        std::vector<std::shared_ptr<Objects::Characters::PlayerCharacter>> Battle::GetParty() {
            std::vector<std::shared_ptr<Objects::Characters::PlayerCharacter>> result;

            std::transform(Services::Locator::WorldService()->GetWorld()->playerParty->characters.begin(),
                           Services::Locator::WorldService()->GetWorld()->playerParty->characters.end(),
                           std::back_inserter(result),
                           [](std::shared_ptr<Objects::Characters::Base> playerCharacter) {
                               return std::static_pointer_cast<Objects::Characters::PlayerCharacter>(playerCharacter);
                           });

            return result;
        }

        std::vector<std::shared_ptr<Objects::Characters::Monster>> Battle::GetMonsters() {
            std::vector<std::shared_ptr<Objects::Characters::Monster>> result;

            std::transform(Services::Locator::WorldService()->GetWorld()->enemyParty->characters.begin(),
                           Services::Locator::WorldService()->GetWorld()->enemyParty->characters.end(),
                           std::back_inserter(result),
                           [](std::shared_ptr<Objects::Characters::Base> monster) {
                               return std::static_pointer_cast<Objects::Characters::Monster>(monster);
                           });

            return result;
        }

        void Battle::LoadLuaState(const std::string& scriptFilePath) {
            Base::LoadLuaState(scriptFilePath);

            this->luaState->open_libraries(sol::lib::math, sol::lib::os, sol::lib::string);

            Objects::Text::LuaInterface::Bind(this->luaState);
            Objects::Image::LuaInterface::Bind(this->luaState);
            Objects::Characters::PlayerCharacter::LuaInterface::Bind(this->luaState);
            Objects::Characters::Monster::LuaInterface::Bind(this->luaState);
            Battle::LuaInterface::Bind(this->luaState);

            this->luaState->set("battle_state", this);

            this->logger->debug() << "Loading \"" << scriptFilePath << "\".";

            this->luaState->script_file(scriptFilePath);

            this->logger->debug() << "Loaded \"" << scriptFilePath << "\".";

            // TODO: Handle errors?
            (*this->luaState.get())["initialize"]();
        }

        void Battle::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
            sol::table states = (*luaState.get())["states"].get_or_create<sol::table>(sol::new_table());

            states.new_usertype<Battle>("Battle",
                                        sol::no_constructor,
                                        "pop", &Battle::Pop,
                                        "processInput", static_cast<std::string (Battle::*)(const int)>(&Battle::ProcessInput),
                                        "getParty", &Battle::GetParty,
                                        "getMonsters", &Battle::GetMonsters
                                        );
        }
    }
}
