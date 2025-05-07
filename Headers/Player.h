#pragma once
#include "BattleBox.h"
#include "StatefulSprite.h"

class Player : public StatefulSprite {
public:
    explicit Player(const std::string &normalPath = "./img/spr_heart_0.png",
           const std::string &hurtPath = "./img/spr_heart_1.png",
           const sf::Vector2f &pos = {312, 232});
    // std::unique_ptr<DrawableEntity> clone() const override;
    friend std::ostream & operator<<(std::ostream &os, const Player &obj);
    void move(const sf::Vector2f& offset);
    void centerPlayer(const BattleBox &battleBox);

};
