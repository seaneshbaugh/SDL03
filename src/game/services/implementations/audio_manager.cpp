#include "audio_manager.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            const std::string AudioManager::logChannel = "audio";

            AudioManager::AudioManager() {
                this->logger = Locator::LoggerService()->GetLogger(AudioManager::logChannel);

                this->mixer = nullptr;

                if (SDL_InitSubSystem(SDL_INIT_AUDIO) == false) {
                    this->logger->critic() << "Failed to initialize SDL audio: " << SDL_GetError();

                    throw;
                }

                this->logger->info() << "SDL audio initialized.";

                if (MIX_Init() == false) {
                    this->logger->critic() << "Failed to initialize SDL_mixer. " << SDL_GetError();

                    throw;
                }

                this->mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);

                if (this->mixer == nullptr) {
                    this->logger->critic() << "Failed to create SDL_mixer mixer device. " << SDL_GetError();

                    throw;
                }

                this->logger->info() << "SDL_mixer mixer device created.";
            }

            AudioManager::~AudioManager() {
                if (this->mixer != nullptr) {
                    MIX_DestroyMixer(this->mixer);

                    this->mixer = nullptr;
                }

                SDL_QuitSubSystem(SDL_INIT_AUDIO);
            }

            MIX_Mixer* AudioManager::GetMixer() {
                return this->mixer;
            }
        }
    }
}
