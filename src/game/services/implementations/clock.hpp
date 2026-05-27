#ifndef SDL03_Game_Services_Implementations_Clock
#define SDL03_Game_Services_Implementations_Clock

#include "../../../game.hpp"
#include "../locator.hpp"
#include "../interfaces/time_service.hpp"

#define MAXIMUM_DELTA_TIME 0.05f

namespace Game {
    namespace Services {
        namespace Implementations {
            class Clock : public Interfaces::TimeService {
            public:
                Clock();
                ~Clock();
                void BeginFrame();
                float GetDeltaTime() const;
                float GetElapsedTime() const;
                void IncrementFrameCount();
                void StartClock(const unsigned int offset);
                unsigned int GetFrameCount();
                std::tuple<unsigned int, unsigned int, unsigned int> GetClockTime();
                float GetGameTimeSeconds() const;

            private:
                static const std::string logChannel;

                Uint64 previousCounter;
                float deltaTime;
                float elapsedTime;
                float offsetSeconds;
                unsigned int frameCount;
                unsigned int startFrame;
                unsigned int offset;

                std::shared_ptr<Log::Logger> logger;
            };
        }
    }
}

#endif
