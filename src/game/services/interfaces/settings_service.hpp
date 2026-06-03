#ifndef SDL03_Services_Interfaces_SettingsService
#define SDL03_Services_Interfaces_SettingsService

#include <map>

#include "../../input/button.hpp"

namespace Game {
    namespace Services {
        namespace Interfaces {
            class SettingsService {
            public:
                virtual ~SettingsService() {}
                virtual std::map<int, Input::Button> InputSettings() = 0;
            };
        }
    }
}

#endif
