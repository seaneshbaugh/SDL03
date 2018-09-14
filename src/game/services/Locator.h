#ifndef __SDL03__Locator__
#define __SDL03__Locator__

#include "interfaces/FontService.h"
#include "interfaces/LoggerService.h"
#include "interfaces/RendererService.h"
#include "interfaces/TextureService.h"
#include "interfaces/WorldService.h"
 
namespace Services {
    class Locator {
    public:
        static std::shared_ptr<Interfaces::FontService> FontService();
        static std::shared_ptr<Interfaces::LoggerService> LoggerService();
        static std::shared_ptr<Interfaces::RendererService> RendererService();
        static std::shared_ptr<Interfaces::TextureService> TextureService();
        static std::shared_ptr<Interfaces::WorldService> WorldService();
        static void ProvideService(std::shared_ptr<Interfaces::FontService> fontService);
        static void ProvideService(std::shared_ptr<Interfaces::LoggerService> loggerService);
        static void ProvideService(std::shared_ptr<Interfaces::RendererService> rendererService);
        static void ProvideService(std::shared_ptr<Interfaces::TextureService> textureService);
        static void ProvideService(std::shared_ptr<Interfaces::WorldService> worldService);
        static void StopServices();
    private:
        static std::shared_ptr<Interfaces::FontService> fontService;
        static std::shared_ptr<Interfaces::LoggerService> loggerService;
        static std::shared_ptr<Interfaces::RendererService> rendererService;
        static std::shared_ptr<Interfaces::TextureService> textureService;
        static std::shared_ptr<Interfaces::WorldService> worldService;
    };
}

#endif
