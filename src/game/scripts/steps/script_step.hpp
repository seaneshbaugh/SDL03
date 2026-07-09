#ifndef SDL03_Game_Scripts_Steps_ScriptStep
#define SDL03_Game_Scripts_Steps_ScriptStep

namespace Game {
    namespace Scripts {
        namespace Steps {
            class ScriptStep {
            public:
                virtual ~ScriptStep();
                virtual void Start() = 0;
                virtual void Update(const float deltaTime) = 0;
                virtual bool IsCompleted() const = 0;
            };
        }
    }
}

#endif
