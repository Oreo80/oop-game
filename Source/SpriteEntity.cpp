#include "../Headers/SpriteEntity.h"

#include <iostream>

sf::Texture SpriteEntity::initTexture(const std::string &texturePath) {
    sf::Texture tex;
    if (!tex.loadFromFile(texturePath)) {
        std::cerr << "Failed to load texture: "<< texturePath << std::endl;
        throw std::runtime_error("Failed to load texture: " + texturePath);
    }
    return tex;
}

SpriteEntity::SpriteEntity(const std::string &texturePath, const sf::Vector2f &pos)
    : texture(initTexture(texturePath)), sprite(texture) {
    sprite.setPosition(pos);
}

std::unique_ptr<DrawableEntity> SpriteEntity::clone() const {
    return std::make_unique<SpriteEntity>(*this);
}

void SpriteEntity::draw(sf::RenderWindow &window) const {
    window.draw(sprite);
}

sf::Vector2f SpriteEntity::getPosition() const {
    return sprite.getPosition();
}

void SpriteEntity::setPosition(const sf::Vector2f &pos) {
    sprite.setPosition(pos);
}

sf::FloatRect SpriteEntity::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f SpriteEntity::getOrigin() const {
    return sprite.getOrigin();
}

void SpriteEntity::setOrigin(const sf::Vector2f &origin_) {
    sprite.setOrigin(origin_);
}

std::ostream & operator<<(std::ostream &os, const SpriteEntity &obj) {
    return os
        << "Position: (" << obj.getPosition().x << ", " << obj.getPosition().y << ")";
}
