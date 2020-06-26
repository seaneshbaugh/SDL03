#ifndef SDL03_Game_States_Base
#define SDL03_Game_States_Base

#include <map>
#include <string>
#include <vector>

#include <SDL.h>

#ifdef __APPLE__
#include "../../../lib/lua/src/lua.h"
#include "../../../lib/lua/src/lualib.h"
#include "../../../lib/lua/src/lauxlib.h"
#else
#include "../../../lib/lua/src/lua.hpp"
#endif
#include "../../../lib/sol/sol.hpp"

#include "../resources/texture.hpp"
#include "../resources/sound.hpp"
#include "../resources/song.hpp"
#include "../parsers/asset_list_parser.hpp"
#include "../objects/text.hpp"
#include "../../helpers/file_system.hpp"

namespace Game {
    namespace States {
        enum class GameStateType {
            intro,
            main_menu,
            settings_menu,
            new_game, // This is not acutally a real state. See WorldManager::NewGame
            map,
            battle,
            pause_menu,
            save_game_menu
        };

        namespace {
            static const std::map<std::string, GameStateType> gameStateTypeMap;
        }

        GameStateType StateNameToEnum(const std::string& stateName);

        class Base : public std::enable_shared_from_this<Base> {
        public:
            bool acceptRawInput;

            virtual ~Base();
            virtual std::shared_ptr<Base> Update(const int key) = 0;
            virtual std::shared_ptr<Base> Update(const SDL_Event& event) = 0;
            virtual std::string ProcessInput(const int key) = 0;
            virtual void Render() = 0;
            virtual void Pop();

        protected:
            std::shared_ptr<Log::Logger> logger;
            bool pop;

            std::shared_ptr<sol::state> luaState;
            std::vector<std::string> textureNames;
            std::vector<std::shared_ptr<Objects::Text>> texts;

            virtual void LoadResources(const std::string& textureListPath, const std::string& soundListPath);
            virtual void LoadResources(const std::string& textureListPath, const std::string& soundListPath, const std::string& songListPath);
            virtual void LoadTextures(const std::string& resourceListPath);
            virtual void LoadSounds(const std::string& resourceListPath);
            virtual void LoadSongs(const std::string& resourceListPath);
            virtual void LoadLuaState(const std::string& scriptFilePath);
            std::shared_ptr<Resources::Texture> GetTexture(const std::string& textureName);
        };
    }
}

#endif
