#include "locator.hpp"

namespace Game {
    namespace Services {
        std::shared_ptr<Interfaces::AudioService> Locator::audioService = nullptr;
        std::shared_ptr<Interfaces::FontService> Locator::fontService = nullptr;
        std::shared_ptr<Interfaces::InputService> Locator::inputService = nullptr;
        std::shared_ptr<Interfaces::LoggerService> Locator::loggerService = nullptr;
        std::shared_ptr<Interfaces::SettingsService> Locator::settingsService = nullptr;
        std::shared_ptr<Interfaces::TextureService> Locator::textureService = nullptr;
        std::shared_ptr<Interfaces::TimeService> Locator::timeService = nullptr;
        std::shared_ptr<Interfaces::VideoService> Locator::videoService = nullptr;
        std::shared_ptr<Interfaces::WorldService> Locator::worldService = nullptr;
        std::shared_ptr<Interfaces::SaveService> Locator::saveService = nullptr;

        std::shared_ptr<Interfaces::AudioService> Locator::AudioService() {
            return Locator::audioService;
        }

        std::shared_ptr<Interfaces::FontService> Locator::FontService() {
            return Locator::fontService;
        }

        std::shared_ptr<Interfaces::InputService> Locator::InputService() {
            return Locator::inputService;
        }

        std::shared_ptr<Interfaces::LoggerService> Locator::LoggerService() {
            return Locator::loggerService;
        }

        std::shared_ptr<Interfaces::SettingsService> Locator::SettingsService() {
            return Locator::settingsService;
        }

        std::shared_ptr<Interfaces::VideoService> Locator::VideoService() {
            return Locator::videoService;
        }

        std::shared_ptr<Interfaces::TextureService> Locator::TextureService() {
            return Locator::textureService;
        }

        std::shared_ptr<Interfaces::TimeService> Locator::TimeService() {
            return Locator::timeService;
        }

        std::shared_ptr<Interfaces::WorldService> Locator::WorldService() {
            return Locator::worldService;
        }

        std::shared_ptr<Interfaces::SaveService> Locator::SaveService() {
            return Locator::saveService;
        }

        void Locator::ProvideService(std::shared_ptr<Interfaces::AudioService> audioService) {
            Locator::audioService = audioService;
        }

        void Locator::ProvideService(std::shared_ptr<Interfaces::FontService> fontService) {
            Locator::fontService = fontService;
        }

        void Locator::ProvideService(std::shared_ptr<Interfaces::InputService> inputService) {
            Locator::inputService = inputService;
        }

        void Locator::ProvideService(std::shared_ptr<Interfaces::LoggerService> loggerService) {
            Locator::loggerService = loggerService;
        }

        void Locator::ProvideService(std::shared_ptr<Interfaces::SettingsService> settingsService) {
            Locator::settingsService = settingsService;
        }

        void Locator::ProvideService(std::shared_ptr<Interfaces::TextureService> textureService) {
            Locator::textureService = textureService;
        }

        void Locator::ProvideService(std::shared_ptr<Interfaces::TimeService> timeService) {
            Locator::timeService = timeService;
        }

        void Locator::ProvideService(std::shared_ptr<Interfaces::VideoService> videoService) {
            Locator::videoService = videoService;
        }

        void Locator::ProvideService(std::shared_ptr<Interfaces::WorldService> worldService) {
            Locator::worldService = worldService;
        }

        void Locator::ProvideService(std::shared_ptr<Interfaces::SaveService> saveService) {
            Locator::saveService = saveService;
        }

        void Locator::StopAudioService() {
            if (Locator::audioService != nullptr) {
                Locator::audioService.reset();
            }
        }

        void Locator::StopFontService() {
            if (Locator::fontService != nullptr) {
                Locator::fontService.reset();
            }
        }

        void Locator::StopInputService() {
            if (Locator::inputService != nullptr) {
                Locator::inputService.reset();
            }
        }

        void Locator::StopLoggerService() {
            if (Locator::loggerService != nullptr) {
                Locator::loggerService.reset();
            }
        }

        void Locator::StopSettingsService() {
            if (Locator::settingsService != nullptr) {
                Locator::settingsService.reset();
            }
        }

        void Locator::StopTextureService() {
            if (Locator::textureService != nullptr) {
                Locator::textureService.reset();
            }
        }

        void Locator::StopTimeService() {
            if (Locator::timeService != nullptr) {
                Locator::timeService.reset();
            }
        }

        void Locator::StopVideoService() {
            if (Locator::videoService != nullptr) {
                Locator::videoService.reset();
            }
        }

        void Locator::StopWorldService() {
            if (Locator::worldService != nullptr) {
                Locator::worldService.reset();
            }
        }

        void Locator::StopSaveService() {
            if (Locator::saveService != nullptr) {
                Locator::saveService.reset();
            }
        }
    }
}
