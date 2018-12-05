#include "audio_manager.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            const std::string AudioManager::logChannel = "audio";

            AudioManager::AudioManager() {
                this->logger = Locator::LoggerService()->GetLogger(AudioManager::logChannel);

                if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
                    this->logger->critic() << "Failed to initialize SDL audio: " << SDL_GetError();

                    throw;
                }

                this->logger->info() << "SDL audio initialized.";

                if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
                    this->logger->critic() << "Failed to initialize SDL_mixer. " << Mix_GetError();

                    throw;
                }

                this->logger->info() << "SDL_mixer initialized.";
            }

            AudioManager::~AudioManager() {
                Mix_CloseAudio();

                SDL_QuitSubSystem(SDL_INIT_AUDIO);
            }
        }
    }
}
