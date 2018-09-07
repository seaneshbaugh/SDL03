#ifndef __SDL03__Locator__
#define __SDL03__Locator__

#include "interfaces/FontService.h"

namespace Services {
    class Locator {
    public:
        static std::shared_ptr<Interfaces::FontService> FontService();
        static void ProvideService(std::shared_ptr<Interfaces::FontService> fontService);
        static void StopServices();
    private:
        static std::shared_ptr<Interfaces::FontService> _FontService;
    };
}

#endif
