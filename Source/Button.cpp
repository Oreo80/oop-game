#include "../Headers/Button.h"

std::ostream & operator<<(std::ostream &os, const Button &obj){
    return os
            << static_cast<const StatefulSprite&>(obj);
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


