#include "clock.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            const std::string Clock::logChannel = "time";

            Clock::Clock() {
                this->logger = Locator::LoggerService()->GetLogger(Clock::logChannel);
                this->frameCount = 0;
                this->startFrame = 0;
                this->offset = 0;
            }

            Clock::~Clock() {
            }

            void Clock::IncrementFrameCount() {
                this->frameCount++;
            }

            void Clock::StartClock(unsigned int offset) {
                this->startFrame = this->frameCount;
            }

            unsigned int Clock::GetFrameCount() {
                return this->frameCount;
            }

            std::tuple<unsigned int, unsigned int, unsigned int> Clock::GetClockTime() {
                static const unsigned int framesPerMinute = 60 * Game::FRAMES_PER_SECOND;
                static const unsigned int framesPerHour = 60 * framesPerMinute;

                const unsigned int elapsedFrames = (this->frameCount - this->startFrame) + this->offset;

                const unsigned int elapsedHours = elapsedFrames / framesPerHour;
                const unsigned int elapsedMinutes = (elapsedFrames - (elapsedHours * framesPerHour)) / framesPerMinute;
                const unsigned int elapsedSeconds = (elapsedFrames - ((elapsedHours * framesPerHour) + (elapsedMinutes * framesPerMinute))) / Game::FRAMES_PER_SECOND;

                return std::make_tuple(elapsedHours, elapsedMinutes, elapsedSeconds);
            }
        }
    }
}
