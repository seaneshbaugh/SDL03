#ifndef SDL03_Game_Services_Interfaces_TimeService
#define SDL03_Game_Services_Interfaces_TimeService

#include <tuple>

namespace Game {
    namespace Services {
        namespace Interfaces {
            class TimeService {
            public:
                virtual ~TimeService() {}
                virtual void IncrementFrameCount() = 0;
                virtual void StartClock(const unsigned int offset) = 0;
                virtual std::tuple<unsigned int, unsigned int, unsigned int> GetClockTime() = 0;
            };
        }
    }
}

#endif
