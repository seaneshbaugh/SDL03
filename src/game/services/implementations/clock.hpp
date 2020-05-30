#ifndef SDL03_Game_Services_Implementations_Clock
#define SDL03_Game_Services_Implementations_Clock

#include "../../../game.hpp"
#include "../locator.hpp"
#include "../interfaces/time_service.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            class Clock : public Interfaces::TimeService {
            public:
                Clock();
                ~Clock();
                void IncrementFrameCount();
                void StartClock(const unsigned int offset);
                std::tuple<unsigned int, unsigned int, unsigned int> GetClockTime();

            private:
                static const std::string logChannel;

                unsigned int frameCount;
                unsigned int startFrame;
                unsigned int offset;

                std::shared_ptr<Log::Logger> logger;
            };
        }
    }
}

#endif
