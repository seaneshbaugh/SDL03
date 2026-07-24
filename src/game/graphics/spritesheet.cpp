#include "spritesheet.hpp"

namespace Game {
    namespace Graphics {
        const std::string Spritesheet::logChannel = "graphics";

        Spritesheet::Spritesheet(const std::string& name) {
            this->logger = Services::Locator::LoggerService()->GetLogger(Spritesheet::logChannel);
            this->name = name;

            if (!this->Load(this->NameToFilename())) {
                this->logger->error() << "Failed to load spritesheet \"" << this->name << "\".";
            }
        }

        Spritesheet::~Spritesheet() {
        }

        std::shared_ptr<AnimationClip> Spritesheet::GetAnimationClip(const std::string& animationClipName) const {
            return this->animationClips.at(animationClipName);
        }

        std::shared_ptr<Assets::Texture> Spritesheet::GetTexture() const {
            return this->texture;
        }

        bool Spritesheet::Load(const std::string& filename) {
            std::string jsonString;

            if (!Helpers::FileSystem::ReadFile(filename, jsonString)) {
                return false;
            }

            if (!this->ParseSpritesheetFile(jsonString)) {
                return false;
            }

            return true;
        }

        bool Spritesheet::ParseSpritesheetFile(const std::string& jsonString) {
            std::unique_ptr<Parser> parser = std::make_unique<Parser>();

            parser->Parse(jsonString, *this);

            return true;
        }

        std::string Spritesheet::NameToFilename() {
            // TODO: This feels very brittle. What I really want is some way of mapping a
            // unique identifier to a spritesheet file path. Need to ponder this more.
            std::string filename = "assets/spritesheets/" + this->name + ".json";
            return filename;
        }

        const std::string Spritesheet::Parser::logChannel = "json";

        Spritesheet::Parser::Parser() {
            this->logger = Services::Locator::LoggerService()->GetLogger(Spritesheet::Parser::logChannel);
        }

        Spritesheet::Parser::~Parser() {
        }

        void Spritesheet::Parser::Parse(const std::string& jsonString, Spritesheet& spritesheet) {
            json spritesheetNode = json::parse(jsonString);

            spritesheet.texture = Services::Locator::TextureService()->AddTexture(spritesheet.name, spritesheetNode["texture"].get<std::string>());
            spritesheet.animationClips = this->ParseAnimationClips(spritesheetNode["animations"]);
        }

        std::map<std::string, std::shared_ptr<AnimationClip>> Spritesheet::Parser::ParseAnimationClips(const json& animationClipsNode) {
            std::map<std::string, std::shared_ptr<AnimationClip>> animationClips;

            for (auto animationClipNode = animationClipsNode.begin(); animationClipNode != animationClipsNode.end(); ++animationClipNode) {
                const std::string animationClipName = animationClipNode.key();
                const json& variants = animationClipNode.value();
                std::map<Direction, std::shared_ptr<Animation>> variantAnimations;

                for (auto variantNode = variants.begin(); variantNode != variants.end(); ++variantNode) {
                    const std::string directionString = variantNode.key();

                    Direction direction;

                    if (directionString == "up") {
                        direction = Direction::Up;
                    } else if (directionString == "right") {
                        direction = Direction::Right;
                    } else if (directionString == "down") {
                        direction = Direction::Down;
                    } else if (directionString == "left") {
                        direction = Direction::Left;
                    } else {
                        direction = Direction::Down;
                    }

                    int width = variantNode.value()["width"].get<int>();
                    int height = variantNode.value()["height"].get<int>();

                    std::vector<Graphics::AnimationFrame> frames;

                    for (auto frameNode = variantNode.value()["frames"].begin(); frameNode != variantNode.value()["frames"].end(); ++frameNode) {
                        frames.push_back(this->ParseAnimationFrame(frameNode.value()));
                    }

                    std::shared_ptr<Graphics::Animation> animation = std::make_shared<Graphics::Animation>(width, height, frames);

                    variantAnimations.insert(std::make_pair(direction, animation));
                }

                std::shared_ptr<AnimationClip> animationClip = std::make_shared<AnimationClip>(variantAnimations);
                animationClips.insert(std::make_pair(animationClipName, animationClip));
            }

            return animationClips;
        }

        AnimationFrame Spritesheet::Parser::ParseAnimationFrame(const json& animationFrameNode) {
            const int offsetX = animationFrameNode[0].get<int>();
            const int offsetY = animationFrameNode[1].get<int>();

            AnimationFrame frame(offsetX, offsetY);

            return frame;
        }
    }
}
