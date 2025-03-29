#include "../Headers/Button.h"

std::ostream & operator<<(std::ostream &os, const Button &obj){
    return os
            << "Position: (" << obj.position.x << ", " << obj.position.y << ")";
}

void Button::draw(sf::RenderWindow& window) const{
    window.draw(sprite);
}
void Button::setTexture(const bool isSelected) {
    if (const std::string &texturePath = isSelected ? selectedTexturePath : normalTexturePath; !texture.
        loadFromFile(texturePath)) {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
        throw std::runtime_error("Failed to load texture "+ texturePath);
    }
    sprite.setTexture(texture);
}

sf::Vector2f Button::getPositionForPlayer() const {
    return { sprite.getPosition().x+8,sprite.getPosition().y+14};
}

