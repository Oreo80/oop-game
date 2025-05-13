#pragma once
#include <map>

#include "SpriteEntity.h"

class StatefulSprite : public SpriteEntity {
private:
    std::map<std::string, sf::Texture> states;
    std::string currentState;
public:
    explicit StatefulSprite(const std::string &defaultPath = "./img/missing.png",
                            const std::string &defaultStateName = "normal",
                            const sf::Vector2f &pos = {0, 0});
    std::unique_ptr<DrawableEntity> clone() const override;
    void addState(const std::string &stateName, const std::string& texturePath);
    void setState(const std::string& stateName);
    friend std::ostream & operator<<(std::ostream &os, const StatefulSprite &obj);
};