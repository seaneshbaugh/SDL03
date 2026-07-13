#ifndef SDL03_Game_Scripts_Steps_ScriptStep
#define SDL03_Game_Scripts_Steps_ScriptStep

namespace Game {
    namespace Scripts {
        class ScriptRunner;

        namespace Steps {
            class ScriptStep {
            public:
                virtual ~ScriptStep();
                virtual void Start(Scripts::ScriptRunner* scriptRunner) = 0;
                virtual void Update(const float deltaTime) = 0;
                virtual void Render() = 0;
                virtual bool IsCompleted() const = 0;
            };
        }
    }
}

#endif
