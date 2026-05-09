#include "clock.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            const std::string Clock::logChannel = "time";

            Clock::Clock() {
                this->logger = Locator::LoggerService()->GetLogger(Clock::logChannel);
                this->previousCounter = SDL_GetPerformanceCounter();
                this->deltaTime = 0.0;
                this->elapsedTime = 0.0;
                this->frameCount = 0;
                this->startFrame = 0;
                this->offset = 0;
            }

            Clock::~Clock() {
            }

            void Clock::BeginFrame() {
                Uint64 currentCounter = SDL_GetPerformanceCounter();

                this->deltaTime = std::min(static_cast<double>(currentCounter - this->previousCounter) / static_cast<double>(SDL_GetPerformanceFrequency()), MAXIMUM_DELTA_TIME);

                this->previousCounter = currentCounter;

                this->elapsedTime += this->deltaTime;
            }

            double Clock::GetDeltaTime() const {
                return this->deltaTime;
            }

            double Clock::GetElapsedTime() const {
                return this->elapsedTime;
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

            double Clock::GetGameTimeSeconds() const {
                return this->elapsedTime + this->offsetSeconds;
            }

            std::tuple<unsigned int, unsigned int, unsigned int> Clock::GetClockTime() {
                unsigned int totalSeconds = static_cast<unsigned int>(this->elapsedTime);

                unsigned int elapsedHours = totalSeconds / 3600;
                unsigned int elapsedMinutes = (totalSeconds % 3600) / 60;
                unsigned int elapsedSeconds = totalSeconds % 60;

                return std::make_tuple(elapsedHours, elapsedMinutes, elapsedSeconds);
            }
        }
    }
}
