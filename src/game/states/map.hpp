#ifndef SDL03_Game_States_Map
#define SDL03_Game_States_Map

#include "base.hpp"
#include "pause_menu.hpp"
#include "battle.hpp"
#include "../objects/maps/map.hpp"
#include "../objects/world.hpp"
#include "../scene/camera.hpp"
#include "../scene/actor_manager.hpp"
#include "../scene/dialogue/dialogue_session.hpp"
#include "../scene/cutscenes/cutscene_session.hpp"

namespace Game {
    namespace States {
        class Map : public Base {
        public:
            friend class Scene::Cutscenes::Actions::PathfindActor;

            enum class State {
                Gameplay,
                Dialogue,
                Cutscene
            };

            std::shared_ptr<Scene::ActorManager> actorManager;
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
            void Step(unsigned int x, unsigned int y);
            void StartDialogue(const std::string& dialogueId);
            bool DialogueSessionCompleted() const;
            void StartCutscene(const std::string& cutsceneId);
            std::shared_ptr<Scene::Actor> AddActorAtSpawnPoint(const std::string& id, const std::string& name, const std::string& spritesheetName, const std::string& dialogueId, const std::string& spawnPointName, const Scene::Actor::Direction direction, const std::string& movementScriptName, const std::string& interactionScriptName);

        protected:
            std::vector<Scene::Actor::Direction> Pathfind(Scene::Actor* actor, const int targetX, const int targetY);

        private:
            static const std::string logChannel;

            Objects::Maps::MapEncounterArea* currentMapEncounterArea;
            std::shared_ptr<Scene::Camera> camera;
            Scene::Actor::Direction movementDirection;
            Scene::Actor::Direction movementInputHeldDirection;
            bool movementInputHeld;
            bool interactionRequested;
            bool dialogueNextPressed;
            bool dialogueChoiceInputPressed;
            Scene::Actor::Direction dialogueChoiceInputDirection;
            Scene::Dialogue::DialogueSession dialogueSession;
            Scene::Cutscenes::CutsceneSession cutsceneSession;
            State state;
            State previousState;

            void UpdateMovementInput();
            // TODO: Go through the rest of the game and change references to "keys" to "buttons".
            // Button is a more accurate term for what these are. They're not necessarily keys on a keyboard. They could be buttons on a gamepad or something else entirely.
            void ProcessButtonDown(const InputKey key);
            void ProcessButtonUp(const InputKey key);
            std::string ProcessInput(const InputKey key);
            Transition UpdateGameplay(const float deltaTime);
            Transition UpdateDialogue(const float deltaTime);
            Transition UpdateCutscene(const float deltaTime);
            void LoadLuaState(const std::string& scriptFilePath);
            void PlaceActor(std::shared_ptr<Scene::Actor> actor, const int x, const int y, const Scene::Actor::Direction direction);
            void QueueMovement(Scene::Actor* actor, const Scene::Actor::Direction direction, const int distance);
            bool CanMove(Scene::Actor* actor, const Scene::Actor::Direction direction);
            bool TryInteract();
            std::optional<std::shared_ptr<Scene::Actor>> GetActorAtTile(const int x, const int y) const;

        public:
            class LuaInterface {
            public:
                static void Bind(std::shared_ptr<sol::state> luaState);
            };
        };
    }
}

#endif
