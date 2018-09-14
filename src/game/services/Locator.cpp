#include "Locator.h"

namespace Services {
    std::shared_ptr<Interfaces::FontService> Locator::fontService = nullptr;
    std::shared_ptr<Interfaces::LoggerService> Locator::loggerService = nullptr;
    std::shared_ptr<Interfaces::RendererService> Locator::rendererService = nullptr;
    std::shared_ptr<Interfaces::TextureService> Locator::textureService = nullptr;
    std::shared_ptr<Interfaces::WorldService> Locator::worldService = nullptr;

    std::shared_ptr<Interfaces::FontService> Locator::FontService() {
        return fontService;
    }

    std::shared_ptr<Interfaces::LoggerService> Locator::LoggerService() {
        return loggerService;
    }

    std::shared_ptr<Interfaces::RendererService> Locator::RendererService() {
        return rendererService;
    }

    std::shared_ptr<Interfaces::TextureService> Locator::TextureService() {
        return textureService;
    }

    std::shared_ptr<Interfaces::WorldService> Locator::WorldService() {
        return worldService;
    }

    void Locator::ProvideService(std::shared_ptr<Interfaces::FontService> fontService) {
        Locator::fontService = fontService;
    }

    void Locator::ProvideService(std::shared_ptr<Interfaces::LoggerService> loggerService) {
        Locator::loggerService = loggerService;
    }

    void Locator::ProvideService(std::shared_ptr<Interfaces::RendererService> rendererService) {
        Locator::rendererService = rendererService;
    }

    void Locator::ProvideService(std::shared_ptr<Interfaces::TextureService> textureService) {
        Locator::textureService = textureService;
    }

    void Locator::ProvideService(std::shared_ptr<Interfaces::WorldService> worldService) {
        Locator::worldService = worldService;
    }

    // TODO: Rethink this.
    void Locator::StopServices() {
        if (fontService != nullptr) {
            fontService.reset();
        }

        if (loggerService != nullptr) {
            loggerService.reset();
        }

        if (rendererService != nullptr) {
            rendererService.reset();
        }

        if (textureService != nullptr) {
            textureService.reset();
        }

        if (worldService != nullptr) {
            worldService.reset();
        }
    }
}
