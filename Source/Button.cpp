#include "../Headers/Button.h"

Button::Button(const std::string &normalPath, const std::string &selectedPath, const sf::Vector2f &pos): StatefulSprite(
    normalPath, "unselected", pos) {
    addState("selected", selectedPath);
}

std::unique_ptr<DrawableEntity> Button::clone() const {
    return std::make_unique<Button>(*this);
}

sf::Vector2f Button::getPositionForPlayer() const {
    return { sprite.getPosition().x+8,sprite.getPosition().y+14};
}

void Button::setSelected(const bool &isSelected) {
    if (isSelected) {
        setState("selected");
    }
    else {
        setState("unselected");
    }
}