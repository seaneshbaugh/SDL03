#ifndef SDL03_Game_States_Map
#define SDL03_Game_States_Map

#include "base.hpp"
#include "pause_menu.hpp"
#include "battle.hpp"
#include "../objects/maps/map.hpp"
#include "../objects/world.hpp"
#include "../scenes/scene.hpp"
#include "../scenes/camera.hpp"
#include "../scenes/actor_manager.hpp"
#include "../scenes/dialogue/dialogue_session.hpp"
#include "../scenes/cutscenes/cutscene_session.hpp"

namespace Game {
    namespace States {
        class Map : public Base {
        public:
            friend class Scenes::Cutscenes::Actions::PathfindActor;

            enum class State {
                Gameplay,
                Dialogue,
                Cutscene
            };

            std::shared_ptr<Scenes::Scene> scene;
            std::shared_ptr<Scenes::Camera> camera;
            std::shared_ptr<Objects::Maps::Map> currentMap;

            Map();
            ~Map();
            void HandleEvent(const SDL_Event& event);
            Transition Update(const float deltaTime);
            void Render();
            bool LoadMap(const std::string& mapName, const int startX, const int startY);
            bool UnloadMap();
            std::shared_ptr<Objects::Maps::Map> GetCurrentMap();
            std::shared_ptr<Objects::Maps::MapEncounterArea> GetCurrentMapEncounterArea(const int x, const int y);
            void SetCurrentMapEncounterArea(Objects::Maps::MapObject* mapEncounterArea);
            bool CanMove(Scenes::Actor* actor, const Scenes::Actor::Direction direction);
            void Step(unsigned int x, unsigned int y);
            void StartDialogue(const std::string& dialogueId);
            bool DialogueSessionCompleted() const;
            void StartCutscene(const std::string& cutsceneId);
            std::shared_ptr<Scenes::Actor> AddActorAtSpawnPoint(const std::string& id, const std::string& name, const std::string& spritesheetName, const std::string& dialogueId, const std::string& spawnPointName, const Scenes::Actor::Direction direction, const std::string& movementScriptName, const std::string& interactionScriptName);

        protected:
            std::vector<Scenes::Actor::Direction> Pathfind(Scenes::Actor* actor, const int targetX, const int targetY);

        private:
            static const std::string logChannel;

            Objects::Maps::MapEncounterArea* currentMapEncounterArea;
            Scenes::Actor::Direction movementDirection;
            Scenes::Actor::Direction movementInputHeldDirection;
            bool movementInputHeld;
            float dialogueChoiceInputTimer;
            Scenes::Dialogue::DialogueSession dialogueSession;
            Scenes::Cutscenes::CutsceneSession cutsceneSession;
            State state;
            State previousState;

            void UpdateMovementInput();
            std::string ProcessInput(const Input::Button key);
            Transition UpdateGameplay(const float deltaTime);
            Transition UpdateDialogue(const float deltaTime);
            Transition UpdateCutscene(const float deltaTime);
            void LoadLuaState(const std::string& scriptFilePath);
            void QueueMovement(Scenes::Actor* actor, const Scenes::Actor::Direction direction, const int distance);
            bool TryInteract();
            std::optional<std::shared_ptr<Scenes::Actor>> GetActorAtTile(const int x, const int y) const;

        public:
            class LuaInterface {
            public:
                static void Bind(std::shared_ptr<sol::state> luaState);
            };
        };
    }
}

#endif
