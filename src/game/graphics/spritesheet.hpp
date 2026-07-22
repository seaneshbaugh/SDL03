#ifndef SDL03_Game_Graphics_Spritesheet
#define SDL03_Game_Graphics_Spritesheet

#include "../../../../lib/nlohmann/json.hpp"

#include "../../helpers/file_system.hpp"
#include "../services/locator.hpp"
#include "animation.hpp"
#include "animation_clip.hpp"

using json = nlohmann::json;

namespace Game {
    namespace Graphics {
        class Spritesheet {
        public:
            std::string name;
            std::map<std::string, std::shared_ptr<AnimationClip>> animationClips;

            std::map<std::string, Animation> animations;
            // std::map<std::string, SpriteRegion> statics;

            Spritesheet(const std::string& name);
            ~Spritesheet();
            SDL_Rect GetSpriteRect(const std::string& animationName, const unsigned int frameIndex);
            std::shared_ptr<AnimationClip> GetAnimationClip(const std::string& animationClipName) const;
            std::shared_ptr<Animation> GetAnimation(const std::string& animationName, const Direction direction) const;
            std::shared_ptr<Assets::Texture> GetTexture() const;
            bool Load(const std::string& filename);
            bool ParseSpritesheetFile(const std::string& jsonString);

        private:
            static const std::string logChannel;

            std::shared_ptr<Log::Logger> logger;
            std::shared_ptr<Assets::Texture> texture;

            std::string NameToFilename();

            class Parser {
            public:
                Parser();
                ~Parser();
                void Parse(const std::string& jsonString, Spritesheet& spritesheet);

            private:
                static const std::string logChannel;

                std::shared_ptr<Log::Logger> logger;

                std::map<std::string, std::shared_ptr<AnimationClip>> ParseAnimationClips(const json& animationClipsNode);
                std::map<std::string, Animation> ParseAnimations(const json& animationsNode);
                AnimationFrame ParseAnimationFrame(const json& animationFrameNode);
            };
        };
    }
}

#endif
