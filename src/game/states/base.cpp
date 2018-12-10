#include "base.hpp"

namespace Game {
    namespace States {
        const std::map<std::string, GameStateType> gameStateTypeMap = {
            {"intro", GameStateType::intro},
            {"main_menu", GameStateType::main_menu},
            {"settings_menu", GameStateType::settings_menu},
            {"new_game", GameStateType::new_game},
            {"map", GameStateType::map},
            {"battle", GameStateType::battle},
            {"pause_menu", GameStateType::pause_menu}
        };

        GameStateType StateNameToEnum(const std::string& stateName) {
            return States::gameStateTypeMap.at(stateName);
        }

        Base::~Base() {
        }

        void Base::LoadResources(const std::string& textureListPath, const std::string& soundListPath) {
            this->LoadTextures(textureListPath);
            this->LoadSounds(soundListPath);
        }

        void Base::LoadResources(const std::string& textureListPath, const std::string& soundListPath, const std::string& songListPath) {
            this->LoadTextures(textureListPath);
            this->LoadSounds(soundListPath);
            this->LoadSongs(songListPath);
        }

        void Base::Pop() {
            for (auto it = this->textureNames.begin(); it != this->textureNames.end(); ++it) {
                Services::Locator::TextureService()->ReleaseTexture(*it);
            }

            this->pop = true;
        }

        void Base::LoadTextures(const std::string& resourceListPath) {
            this->logger->debug() << "Loading textures from \"" << resourceListPath << "\"";

            std::string jsonString;

            if (!Helpers::FileSystem::ReadFile(resourceListPath, jsonString)) {
                this->logger->error() << "Error: Failed to load resource list \"" << resourceListPath << "\".";

                throw;
            }

            // TODO: Use ResourceList to handle this. One thing at a time though...
            Parsers::AssetListParser parser = Parsers::AssetListParser();
            std::map<std::string, std::string> assetList = parser.Parse(jsonString);

            for (auto it = assetList.begin(); it != assetList.end(); ++it) {
                //std::shared_ptr<Resources::Texture> texture = std::make_shared<Resources::Texture>(it->second);

                auto texture = Services::Locator::TextureService()->AddTexture(it->first, it->second);

                this->textureNames.push_back(it->first);
            }
        }

        void Base::LoadSounds(const std::string& resourceListPath) {
            this->logger->debug() << "Loading sounds from \"" << resourceListPath << "\"";

            std::string jsonString;

            if (!Helpers::FileSystem::ReadFile(resourceListPath, jsonString)) {
                this->logger->error() << "Error: Failed to load resource list \"" << resourceListPath << "\".";

                throw;
            }

            Parsers::AssetListParser parser = Parsers::AssetListParser();
            std::map<std::string, std::string> assetList = parser.Parse(jsonString);

            for (auto it = assetList.begin(); it != assetList.end(); ++it) {
                std::shared_ptr<Resources::Sound> sound = std::make_shared<Resources::Sound>(it->second);

                // TODO: Actually add ability to add sounds to audio service.
                // Services::Locator::AudioService()->AddSound(sound, it->first, shared_from_this());
            }
        }

        void Base::LoadSongs(const std::string& resourceListPath) {
            this->logger->debug() << "Loading sounds from \"" << resourceListPath << "\"";

            std::string jsonString;

            if (!Helpers::FileSystem::ReadFile(resourceListPath, jsonString)) {
                this->logger->error() << "Error: Failed to load resource list \"" << resourceListPath << "\".";

                throw;
            }

            Parsers::AssetListParser parser = Parsers::AssetListParser();
            std::map<std::string, std::string> assetList = parser.Parse(jsonString);

            for (auto it = assetList.begin(); it != assetList.end(); ++it) {
                std::shared_ptr<Resources::Song> song = std::make_shared<Resources::Song>(it->second);

                // TODO: Actually add ability to add songs to audio service.
                // Services::Locator::AudioService()->AddSound(song, it->first, shared_from_this());
            }
        }

        std::shared_ptr<Resources::Texture> Base::GetTexture(const std::string& textureName) {
            return Services::Locator::TextureService()->GetTexture(textureName);
        }
    }
}
