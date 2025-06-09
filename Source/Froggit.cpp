#include "../Headers/Froggit.h"


Froggit::Froggit(const sf::Vector2f &pos)
: Enemy({"./img/spr_froglegs_0.png", "./img/spr_froglegs_1.png"}, pos, 1000, "./json/froggit.json"),
headFrames(loadTextures({"./img/spr_froghead_0.png", "./img/spr_froghead_1.png"})),
headSprite(*headFrames[0])
{
    headSprite.setPosition(pos);
}


void Froggit::update() {
    Enemy::update();

    if (headAnimationClock.getElapsedTime().asMilliseconds() > headCycleTime) {
        currentHeadFrame = (currentHeadFrame + 1) % headFrames.size();
        headSprite.setTexture(*headFrames[currentHeadFrame]);
        headAnimationClock.restart();
    }

    const float totalTime = moveClock.getElapsedTime().asSeconds();
    const float offsetX = amplitude * std::sin(speedX * totalTime + phaseOffset);
    const float offsetY = amplitude * std::cos(speedY * totalTime);

    headSprite.setPosition({
        sprite.getPosition().x + offsetX,
        sprite.getPosition().y + offsetY + 3
    });
}

void Froggit::draw(sf::RenderWindow &window) const {
    Enemy::draw(window);
    window.draw(headSprite);
}
