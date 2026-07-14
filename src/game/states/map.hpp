#ifndef SDL03_Game_States_Map
#define SDL03_Game_States_Map

#include "base.hpp"
#include "pause_menu.hpp"
#include "battle.hpp"
#include "../interfaces/command_queue.hpp"
#include "../objects/maps/map.hpp"
#include "../world/state.hpp"
#include "../scenes/scene.hpp"
#include "../scenes/camera.hpp"
#include "../scenes/actor_manager.hpp"
#include "../scripts/script_runner.hpp"

namespace Game {
    namespace States {
        class Map : public Base, public Interfaces::CommandQueue {
        public:
            enum class State {
                Gameplay,
                Script
            };

            std::shared_ptr<Scenes::Scene> scene;
            std::shared_ptr<Objects::Maps::Map> currentMap;

            Map();
            ~Map();
            void HandleEvent(const SDL_Event& event);
            Transition Update(const float deltaTime);
            void Render();
            bool LoadMap(const std::string& mapName, const int startX, const int startY);
            bool UnloadMap();
            std::shared_ptr<Objects::Maps::Map> GetCurrentMap() const;
            std::shared_ptr<Objects::Maps::MapEncounterArea> GetCurrentMapEncounterArea(const int x, const int y) const;
            void SetCurrentMapEncounterArea(Objects::Maps::MapObject* mapEncounterArea);
            void StartScript(const std::string& cutsceneId);
            std::shared_ptr<Scenes::Actor> AddActorAtSpawnPoint(const std::string& id, const std::string& name, const std::string& spritesheetName, const std::string& dialogueProfileId, const std::string& spawnPointName, const Scenes::Actor::Direction direction, const std::string& movementScriptName, const std::string& interactionScriptName);

        private:
            static const std::string logChannel;

            Scripts::ScriptRunner scriptRunner;
            Objects::Maps::MapEncounterArea* currentMapEncounterArea;
            State state;
            State previousState;
            Scenes::Actor* currentInteractingActor;

            std::string ProcessInput(const Input::Button key);
            Transition UpdateGameplay(const float deltaTime);
            Transition UpdateScript(const float deltaTime);
            void ProcessPendingCommands() override;
            void LoadLuaState(const std::string& scriptFilePath);

        public:
            class LuaInterface {
            public:
                static void Bind(std::shared_ptr<sol::state> luaState);
            };
        };
    }
}

#endif
