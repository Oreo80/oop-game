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
                               const sf::Vector2f &pos, const int interval): frames(loadTextures(paths)), sprite(*frames[0]), animationInterval(interval) {
    sprite.setPosition(pos);
}

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

void AnimatedEntity::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

sf::Vector2f AnimatedEntity::getPosition() const {
    return sprite.getPosition();
}

void AnimatedEntity::setPosition(const sf::Vector2f& pos) {
    sprite.setPosition(pos);
}

sf::FloatRect AnimatedEntity::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

std::ostream & operator<<(std::ostream &os, const AnimatedEntity &obj) {
    obj.print(os);
    return os;
}