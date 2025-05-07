#include "../Headers/StatefulSprite.h"

StatefulSprite::StatefulSprite(const std::string &defaultPath, const std::string &defaultStateName,
    const sf::Vector2f &pos): SpriteEntity(defaultPath, pos), currentState(defaultStateName) {
    states[defaultStateName] = texture;
}

// std::unique_ptr<DrawableEntity> StatefulSprite::clone() const {
//     return std::make_unique<StatefulSprite>(*this);
// }

void StatefulSprite::addState(const std::string &stateName, const std::string &texturePath) {
    sf::Texture tex;
    if (!tex.loadFromFile(texturePath)) throw std::runtime_error("Failed to load "+texturePath);
    states[stateName] = std::move(tex);
}

void StatefulSprite::setState(const std::string &stateName) {
    const auto stateEntryIt = states.find(stateName);
    if (stateEntryIt == states.end()) throw std::runtime_error("Unknown state "+stateName);
    currentState = stateName;
    sprite.setTexture(stateEntryIt->second);
}

std::ostream & operator<<(std::ostream &os, const StatefulSprite &obj){
    return os
            << static_cast<const SpriteEntity&>(obj)<< " , Current state : " << obj.currentState;
}