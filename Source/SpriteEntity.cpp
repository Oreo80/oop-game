#include "../Headers/SpriteEntity.h"
#include "../Headers/ResourceManager.h"
#include <iostream>

void SpriteEntity::print(std::ostream &os) const {
    os << "Position: (" << getPosition().x << ", " << getPosition().y << ")";
}

SpriteEntity::SpriteEntity(const std::string &texturePath, const sf::Vector2f &pos)
    : texture(ResourceManager<sf::Texture>().get(texturePath)), sprite(*texture) {
    sprite.setPosition(pos);
}

SpriteEntity::SpriteEntity(const SpriteEntity &other): DrawableEntity(other),
                                                       texture(other.texture),
                                                       sprite(other.sprite) {
}

SpriteEntity::SpriteEntity(SpriteEntity &&other) noexcept: DrawableEntity(std::move(other)),
                                                           texture(std::move(other.texture)),
                                                           sprite(std::move(other.sprite)) {
}

SpriteEntity & SpriteEntity::operator=(SpriteEntity other) {
    using std::swap;
    swap(*this, other);
    return *this;
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
