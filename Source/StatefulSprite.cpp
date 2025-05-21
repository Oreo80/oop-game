#include "../Headers/StatefulSprite.h"

#include "../Headers/ResourceManager.h"
#include "../Headers/Exceptions.h"

void StatefulSprite::print(std::ostream &os) const {
    SpriteEntity::print(os);
    os << " , Current state : " << currentState;
}

StatefulSprite::StatefulSprite(const std::string &defaultPath, const std::string &defaultStateName,
                               const sf::Vector2f &pos): SpriteEntity(defaultPath, pos), currentState(defaultStateName) {
    states[defaultStateName] = *texture;
}

std::unique_ptr<DrawableEntity> StatefulSprite::clone() const {
    return std::make_unique<StatefulSprite>(*this);
}

void StatefulSprite::addState(const std::string &stateName, const std::string &texturePath) {
    const std::shared_ptr<sf::Texture> tex = ResourceManager<sf::Texture>().get(texturePath);
    states[stateName] = std::move(*tex);
}

void StatefulSprite::setState(const std::string &stateName) {
    const auto stateEntryIt = states.find(stateName);
    if (stateEntryIt == states.end()) throw InvalidStateException(stateName);
    currentState = stateName;
    sprite.setTexture(stateEntryIt->second);
}
