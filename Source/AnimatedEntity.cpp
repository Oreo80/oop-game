#include "../Headers/AnimatedEntity.h"



std::vector<std::shared_ptr<sf::Texture>> AnimatedEntity::loadTextures(
    const std::vector<std::string>& paths)
{
    std::vector<std::shared_ptr<sf::Texture>> textures;
    for (const auto& path : paths) {
        textures.push_back(TextureCache::get(path));
    }
    return textures;
}

void AnimatedEntity::updateAnimation() {
    if (animationClock.getElapsedTime().asMilliseconds() > 100) {
        currentFrame = (currentFrame + 1) % frames.size();
        sprite.setTexture(*frames[currentFrame]);
        animationClock.restart();
    }
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
    return os << "Position: (" << obj.getPosition().x << ", " << obj.getPosition().y << ")"
              << "Current frame: "<< obj.currentFrame;
}