#include "sound.hpp"

namespace Game {
    namespace Resources {
        const std::string Sound::logChannel = "assets.sounds";

        Sound::Sound() {
            this->logger = Services::Locator::LoggerService()->GetLogger(Sound::logChannel);
            this->chunk = nullptr;
            this->filename = "";
        }

        Sound::Sound(const std::string& filename) : Sound() {
            this->Load(filename);
        }

        Sound::~Sound() {
            this->DestroyChunk();
        }

        void Sound::Load(const std::string& filename) {
            this->DestroyChunk();

            this->filename = filename;

            this->chunk = Mix_LoadWAV(this->filename.c_str());

            if (this->chunk == nullptr) {
                this->logger->critic() << "Error creating chunk from \"" << filename << "\":" << Mix_GetError();

                throw;
            }
        }

        bool Sound::DestroyChunk() {
            if (this->chunk == nullptr) {
                return false;
            }

            this->logger->info() << "Destroying chunk \"" << this->filename << "\".";

            Mix_FreeChunk(this->chunk);

            this->logger->info() << "Destroyed chunk \"" << this->filename << "\".";

            this->chunk = nullptr;

            return true;
        }
    }
}
