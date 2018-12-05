#ifndef SDL03_Game_Resources_Texture
#define SDL03_Game_Resources_Texture

#include "SDL.h"
#include "SDL_image.h"
#include "luaintf.h"

#include "base.hpp"

namespace Game {
    class Engine;

    namespace Resources {
        class Texture : public Base {
        public:
            SDL_Texture* texture;

            Texture();
            Texture(const std::string& filename);
            ~Texture();
            void Load(const std::string& filename);

        private:
            static const std::string logChannel;

            std::shared_ptr<Log::Logger> logger;
            std::string filename;

            bool DestroyTexture();

            class LuaInterface {
            public:
                static void Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext);
            };
        };
    }
}

#endif
