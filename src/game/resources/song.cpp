#include "song.hpp"

namespace Game {
    namespace Resources {
        Song::Song() {
            this->audio = nullptr;
            this->filename = "";
        }

        Song::Song(const std::string& filename) : Song() {
            this->Load(filename);
        }

        Song::~Song() {
            this->DestroyAudio();
        }

        void Song::Load(const std::string& filename) {
            this->DestroyAudio();

            this->filename = filename;

            this->audio = MIX_LoadAudio(Services::Locator::AudioService()->GetMixer(), this->filename.c_str(), true);

            if (this->audio == nullptr) {
                this->logger->critic() << "Error creating audio from \"" << filename << "\":" << SDL_GetError();

                throw;
            }
        }

        bool Song::DestroyAudio() {
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
