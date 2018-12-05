#include "song.hpp"

namespace Game {
    namespace Resources {
        Song::Song() {
            this->music = nullptr;

            this->filename = "";
        }

        Song::Song(const std::string& filename) : Song() {
            this->Load(filename);
        }

        Song::~Song() {
            this->DestroyMusic();
        }

        void Song::Load(const std::string& filename) {
            this->DestroyMusic();

            this->filename = filename;

            this->music = Mix_LoadMUS(filename.c_str());

            if (this->music == nullptr) {
                this->logger->error() << "Error creating music from \"" << filename << "\":" << Mix_GetError();
            }
        }

        bool Song::DestroyMusic() {
            if (this->music == nullptr) {
                return false;
            }

            this->logger->info() << "Destroying music \"" << this->filename << "\".";

            Mix_FreeMusic(this->music);

            this->logger->info() << "Destroyed music \"" << this->filename << "\".";

            this->music = nullptr;

            return true;
        }
    }
}
