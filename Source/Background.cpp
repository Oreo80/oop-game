#include "../Headers/Background.h"

std::ostream & operator<<(std::ostream &os, const Background &obj){
    return os
            << "file path of loaded texture: "<< obj.texturePath;
}

void Background::draw(sf::RenderWindow& window) const{
    window.draw(sprite);
}