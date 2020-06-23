#ifndef SDL03_Game_Services_Interfaces_SaveService
#define SDL03_Game_Services_Interfaces_SaveService

namespace Game {
    namespace Services {
        namespace Interfaces {
            class SaveService {
            public:
                ~SaveService() {}
                virtual bool SaveGame(const unsigned int slot) = 0;
            };
        }
    }
}

#endif
