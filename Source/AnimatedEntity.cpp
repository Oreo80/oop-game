#include "../Headers/AnimatedEntity.h"

std::vector<std::shared_ptr<sf::Texture>> AnimatedEntity::loadTextures(
    const std::vector<std::string>& paths)
{
    static ResourceManager<sf::Texture> textureCache;
    std::vector<std::shared_ptr<sf::Texture>> textures;
    textures.reserve(paths.size());

    for (const auto& path : paths) {
        textures.push_back(textureCache.get(path));
    }

    return textures;
}

void AnimatedEntity::print(std::ostream &os) const {
    os << "Position: (" << getPosition().x << ", " << getPosition().y << ")"
            << "Current frame: "<< currentFrame;
}

AnimatedEntity::AnimatedEntity(const std::vector<std::string> &paths,
                               const sf::Vector2f &pos, const int interval): SpriteEntity(paths[0],pos), frames(loadTextures(paths)),animationInterval(interval) {}

std::unique_ptr<DrawableEntity> AnimatedEntity::clone() const {
    return std::make_unique<AnimatedEntity>(*this);
}

void AnimatedEntity::animate() {
    if (animationClock.getElapsedTime().asMilliseconds() > animationInterval) {
        currentFrame = (currentFrame + 1) % frames.size();
        sprite.setTexture(*frames[currentFrame]);
        animationClock.restart();
    }
}

void AnimatedEntity::update() {
    animate();
}