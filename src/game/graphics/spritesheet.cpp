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
            const unsigned int offsetX = animationFrameNode[0].get<unsigned int>();
            const unsigned int offsetY = animationFrameNode[1].get<unsigned int>();

            AnimationFrame frame(offsetX, offsetY);

            return frame;
        }
    }
}
