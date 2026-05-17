#ifndef SDL03_Game_Graphics_Spritesheet
#define SDL03_Game_Graphics_Spritesheet

#include "../services/locator.hpp"
#include "animation.hpp"

namespace Game {
    namespace Graphics {
        class Spritesheet {
        public:
            Spritesheet(const std::string Name);
            ~Spritesheet();

            std::string name;
            std::map<std::string, Animation> animations;
            std::map<std::string, SpriteRegion> statics;

        private:
            std::string filepath;
        };
    }
}

#endif
