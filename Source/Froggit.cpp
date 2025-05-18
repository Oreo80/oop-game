#include "../Headers/Froggit.h"

Froggit::Froggit(const std::vector<std::string> &bodyPaths, const std::vector<std::string> &headPaths,
    const sf::Vector2f &pos): AnimatedEntity(bodyPaths, pos, 1000.f),
                              headFrames(loadTextures(headPaths)), headSprite(*headFrames[0]) {
    headSprite.setPosition(pos);
}

void Froggit::update() {
    AnimatedEntity::update();

    if(headAnimationClock.getElapsedTime().asMilliseconds() > headCycleTime) {
        currentHeadFrame = (currentHeadFrame + 1) % headFrames.size();
        headSprite.setTexture(*headFrames[currentHeadFrame]);
        headAnimationClock.restart();
    }

    const float totalTime = moveClock.getElapsedTime().asSeconds();
    const float offsetX = amplitude * std::sin(speedX * totalTime + phaseOffset);
    const float offsetY = amplitude * std::cos(speedY * totalTime);

    headSprite.setPosition({sprite.getPosition().x + offsetX , sprite.getPosition().y + offsetY + 3
    });
}

void Froggit::draw(sf::RenderWindow &window) const {
    AnimatedEntity::draw(window);
    window.draw(headSprite);
}

std::ostream & operator<<(std::ostream &os, const Froggit &obj) {
    obj.print(os);
    return os;
}
