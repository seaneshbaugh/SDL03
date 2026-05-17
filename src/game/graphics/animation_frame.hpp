#ifndef SDL03_Game_Objects_Characters_Animation_Frame
#define SDL03_Game_Objects_Characters_Animation_Frame

namespace Game {
    namespace Graphics {
        class AnimationFrame {
        public:
            unsigned int offsetX;
            unsigned int offsetY;

            AnimationFrame(const unsigned int offsetX, const unsigned int offsetY);
        };
    }
}

#endif
