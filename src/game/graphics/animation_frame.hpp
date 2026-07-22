#ifndef SDL03_Game_Objects_Characters_Animation_Frame
#define SDL03_Game_Objects_Characters_Animation_Frame

namespace Game {
    namespace Graphics {
        class AnimationFrame {
        public:
            int offsetX;
            int offsetY;

            AnimationFrame(const int offsetX, const int offsetY);
        };
    }
}

#endif
