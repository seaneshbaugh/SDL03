#include "sound.hpp"

namespace Game {
    namespace Resources {
        const std::string Sound::logChannel = "assets.sounds";

        Sound::Sound() {
            this->logger = Services::Locator::LoggerService()->GetLogger(Sound::logChannel);
            this->audio = nullptr;
            this->filename = "";
        }

        Sound::Sound(const std::string& filename) : Sound() {
            this->Load(filename);
        }

        Sound::~Sound() {
            this->DestroyAudio();
        }

        void Sound::Load(const std::string& filename) {
            this->DestroyAudio();

            this->filename = filename;

            this->audio = MIX_LoadAudio(Services::Locator::AudioService()->GetMixer(), this->filename.c_str(), true);

            if (this->audio == nullptr) {
                this->logger->critic() << "Error creating audio from \"" << filename << "\":" << SDL_GetError();

                throw;
            }
        }

        bool Sound::DestroyAudio() {
            if (this->audio == nullptr) {
                return false;
            }

            this->logger->info() << "Destroying audio \"" << this->filename << "\".";

            MIX_DestroyAudio(this->audio);

            this->logger->info() << "Destroyed audio \"" << this->filename << "\".";

            this->audio = nullptr;

            return true;
        }
    }
}
