#pragma once
#include "StatefulSprite.h"

class Button : public StatefulSprite {
public:
    explicit Button(const std::string &normalPath = "./img/missing.png",
           const std::string &selectedPath = "./img/missing.png",
           const sf::Vector2f &pos = {0, 0}) : StatefulSprite(normalPath, "unselected", pos) {
        addState("selected", selectedPath);
    }

    friend std::ostream & operator<<(std::ostream &os, const Button &obj);
    sf::Vector2f getPositionForPlayer() const;
    void setSelected(const bool &isSelected);
};
