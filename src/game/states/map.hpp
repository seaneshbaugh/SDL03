#ifndef SDL03_Game_States_Map
#define SDL03_Game_States_Map

#include "base.hpp"
#include "pause_menu.hpp"
#include "battle.hpp"
#include "../objects/maps/map.hpp"
#include "../objects/world.hpp"
#include "../camera.hpp"
#include "../actor.hpp"

namespace Game {
    namespace States {
        class Map : public Base {
        public:
            Map();
            ~Map();
            void HandleEvent(const SDL_Event& event);
            Transition Update(const double deltaTime);
            void Render();
            bool LoadMap(const std::string& mapName, const int startX, const int startY);
            bool UnloadMap();
            std::shared_ptr<Objects::Maps::Map> GetCurrentMap();
            std::shared_ptr<Objects::Maps::MapEncounterArea> GetCurrentMapEncounterArea(const int x, const int y);
            void SetCurrentMapEncounterArea(Objects::Maps::MapObject* mapEncounterArea);
            std::string GetPlayerSpriteName();
            void Step(unsigned int x, unsigned int y);

        private:
            static const std::string logChannel;

            std::shared_ptr<Objects::Maps::Map> currentMap;
            Objects::Maps::MapEncounterArea* currentMapEncounterArea;
            std::unique_ptr<Camera> camera;
            std::shared_ptr<Actor> player;
            std::vector<std::shared_ptr<Actor>> npcs;
            int movementDirection;
            int movementInputHeldDirection;
            bool movementInputHeld;

            void UpdateMovementInput();
            // TODO: Go through the rest of the game and change references to "keys" to "buttons".
            // Button is a more accurate term for what these are. They're not necessarily keys on a keyboard. They could be buttons on a gamepad or something else entirely.
            void ProcessButtonDown(const InputKey key);
            void ProcessButtonUp(const InputKey key);
            std::string ProcessInput(const InputKey key); 
            void LoadLuaState(const std::string& scriptFilePath);
            std::string PlayerSpriteDirection();
            void PlaceActor(std::shared_ptr<Actor> actor, const int x, const int y);

        public:
            class LuaInterface {
            public:
                static void Bind(std::shared_ptr<sol::state> luaState);
            };
        };
    }
}

#endif
