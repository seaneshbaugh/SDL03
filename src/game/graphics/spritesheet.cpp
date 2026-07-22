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

        SDL_Rect Spritesheet::GetSpriteRect(const std::string& animationName, const unsigned int frameIndex) {
            const Animation& animation = this->animations.at(animationName);
            const AnimationFrame& frame = animation.frames.at(frameIndex);
            const SDL_Rect rect = {frame.offsetX, frame.offsetY, animation.width, animation.height};

            return rect;
        }

        std::shared_ptr<AnimationClip> Spritesheet::GetAnimationClip(const std::string& animationClipName) const {
            return this->animationClips.at(animationClipName);
        }

        std::shared_ptr<Graphics::Animation> Spritesheet::GetAnimation(const std::string& animationName, const Direction direction) const {
            return this->animationClips.at(animationName)->GetAnimation(direction);
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
            spritesheet.animations = this->ParseAnimations(spritesheetNode["animations"]);
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

        std::map<std::string, Animation> Spritesheet::Parser::ParseAnimations(const json& animationsNode) {
            std::map<std::string, Animation> animations;

            for (auto animationNode = animationsNode.begin(); animationNode != animationsNode.end(); ++animationNode) {
                const std::string animationName = animationNode.key();

                // animationNode.value() is a json object for this animation
                const json& directions = animationNode.value();

                for (auto animationDirectionNode = directions.begin(); animationDirectionNode != directions.end(); ++animationDirectionNode) {
                    const std::string animationDirection = animationDirectionNode.key();

                    if (animationDirection != "up" && animationDirection != "down" && animationDirection != "left" && animationDirection != "right") {
                        this->logger->error() << "Invalid animation direction \"" << animationDirection << "\" for animation \"" << animationName << "\".";
                        continue;
                    }

                    unsigned int width = animationDirectionNode.value()["width"].get<unsigned int>();
                    unsigned int height = animationDirectionNode.value()["height"].get<unsigned int>();

                    std::vector<Graphics::AnimationFrame> frames;

                    for (auto frameNode = animationDirectionNode.value()["frames"].begin(); frameNode != animationDirectionNode.value()["frames"].end(); ++frameNode) {
                        frames.push_back(this->ParseAnimationFrame(frameNode.value()));
                    }

                    Graphics::Animation animation(width, height, frames);

                    animations.insert(std::make_pair(animationName + "." + animationDirection, animation));
                }
            }

            return animations;
        }

        AnimationFrame Spritesheet::Parser::ParseAnimationFrame(const json& animationFrameNode) {
            const int offsetX = animationFrameNode[0].get<int>();
            const int offsetY = animationFrameNode[1].get<int>();

            AnimationFrame frame(offsetX, offsetY);

            return frame;
        }
    }
}
