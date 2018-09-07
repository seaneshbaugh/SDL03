#include "Locator.h"


namespace Services {
    std::shared_ptr<Interfaces::FontService> Locator::_FontService = nullptr;

    std::shared_ptr<Interfaces::FontService> Locator::FontService() {
        return _FontService;
    }

    void Locator::ProvideService(std::shared_ptr<Interfaces::FontService> fontService) {
        _FontService = fontService;
    }

    // TODO: Rethink this.
    void Locator::StopServices() {
        if (_FontService != nullptr) {
            _FontService.reset();
        }
    }
}
