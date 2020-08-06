#ifndef SDL03_Game_Services_Locator
#define SDL03_Game_Services_Locator

#include <memory>

#include "interfaces/audio_service.hpp"
#include "interfaces/font_service.hpp"
#include "interfaces/input_service.hpp"
#include "interfaces/item_service.hpp"
#include "interfaces/logger_service.hpp"
#include "interfaces/settings_service.hpp"
#include "interfaces/texture_service.hpp"
#include "interfaces/time_service.hpp"
#include "interfaces/video_service.hpp"
#include "interfaces/world_service.hpp"
#include "interfaces/save_service.hpp"

namespace Game {
    namespace Services {
        class Locator {
        public:
            static std::shared_ptr<Interfaces::AudioService> AudioService();
            static std::shared_ptr<Interfaces::FontService> FontService();
            static std::shared_ptr<Interfaces::InputService> InputService();
            static std::shared_ptr<Interfaces::ItemService> ItemService();
            static std::shared_ptr<Interfaces::LoggerService> LoggerService();
            static std::shared_ptr<Interfaces::SettingsService> SettingsService();
            static std::shared_ptr<Interfaces::TextureService> TextureService();
            static std::shared_ptr<Interfaces::TimeService> TimeService();
            static std::shared_ptr<Interfaces::VideoService> VideoService();
            static std::shared_ptr<Interfaces::WorldService> WorldService();
            static std::shared_ptr<Interfaces::SaveService> SaveService();
            static void ProvideService(std::shared_ptr<Interfaces::AudioService> audioService);
            static void ProvideService(std::shared_ptr<Interfaces::FontService> fontService);
            static void ProvideService(std::shared_ptr<Interfaces::InputService> inputService);
            static void ProvideService(std::shared_ptr<Interfaces::ItemService> itemService);
            static void ProvideService(std::shared_ptr<Interfaces::LoggerService> loggerService);
            static void ProvideService(std::shared_ptr<Interfaces::SettingsService> settingsService);
            static void ProvideService(std::shared_ptr<Interfaces::TextureService> textureService);
            static void ProvideService(std::shared_ptr<Interfaces::TimeService> timeService);
            static void ProvideService(std::shared_ptr<Interfaces::VideoService> videoService);
            static void ProvideService(std::shared_ptr<Interfaces::WorldService> worldService);
            static void ProvideService(std::shared_ptr<Interfaces::SaveService> saveService);
            static void StopAudioService();
            static void StopFontService();
            static void StopInputService();
            static void StopItemService();
            static void StopLoggerService();
            static void StopSettingsService();
            static void StopTextureService();
            static void StopTimeService();
            static void StopVideoService();
            static void StopWorldService();
            static void StopSaveService();

        private:
            static std::shared_ptr<Interfaces::AudioService> audioService;
            static std::shared_ptr<Interfaces::FontService> fontService;
            static std::shared_ptr<Interfaces::InputService> inputService;
            static std::shared_ptr<Interfaces::ItemService> itemService;
            static std::shared_ptr<Interfaces::LoggerService> loggerService;
            static std::shared_ptr<Interfaces::SettingsService> settingsService;
            static std::shared_ptr<Interfaces::TextureService> textureService;
            static std::shared_ptr<Interfaces::TimeService> timeService;
            static std::shared_ptr<Interfaces::VideoService> videoService;
            static std::shared_ptr<Interfaces::WorldService> worldService;
            static std::shared_ptr<Interfaces::SaveService> saveService;
        };
    }
}

#endif
