#include "../Headers/Button.h"

std::ostream & operator<<(std::ostream &os, const Button &obj){
    return os
            << "Position: (" << obj.position.x << ", " << obj.position.y << ")";
}

void Button::draw(sf::RenderWindow& window) const{
    window.draw(sprite);
}

