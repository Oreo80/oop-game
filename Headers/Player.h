#pragma once
#include "BattleBox.h"
#include "StatefulSprite.h"

class Player : public StatefulSprite {
private:
    int hurtFramesRemaining = 0;
    static constexpr int framesPerPhase = 2;
    static constexpr int repeatCount = 7;
    void updateHurtAnimation();
protected:
    void print(std::ostream& os) const override;
public:
    explicit Player(const std::string &normalPath = "./img/spr_heart_0.png",
           const std::string &hurtPath = "./img/spr_heart_1.png",
           const sf::Vector2f &pos = {312, 232});
    //cppcheck-suppress unusedFunction
    std::unique_ptr<DrawableEntity> clone() const override;
    void move(const sf::Vector2f& offset);
    void centerPlayer(const BattleBox &battleBox);
    void startHurtAnimation();
    bool isHurting() const;
    void update() override;
};
