#include "../Headers/EnemyTurnSystem.h"

#include <bits/random.h>

#include "../Headers/BulletPattern.h"
EnemyTurnSystem::EnemyTurnSystem(Player& player, 
                                 BattleUISystem& ui, 
                                 BulletManager& bulletManager,
                                 DeathAnimationManager& deathManager,
                                 std::set<sf::Keyboard::Scancode>& keysPressed,
                                 GameManager& gameManager)
    : player(player), 
      ui(ui), 
      bulletManager(bulletManager),
      deathManager(deathManager),
      keysPressed(keysPressed),
      gameManager(gameManager){}

void EnemyTurnSystem::start() {
    player.setState("normal");
    ui.setBattleText("");
    keysPressed.clear();
    player.centerPlayer(ui.getBattleBox());
    enemyTurnClock.restart();
    ui.resizeBattleBox({-205 * 2, 0});

    bulletManager.spawnRandomPattern(player.getPosition());
}

bool EnemyTurnSystem::update() const {
    sf::Vector2f moveOffset = calculateMoveOffset();
    enforceBattleBoxBounds(moveOffset);
    player.move(moveOffset);
    processDamage();

    if (!areBulletsActive() || enemyTurnClock.getElapsedTime().asSeconds() >= enemyTurnDuration) {
        return true;
    }
    return false;
}

sf::Vector2f EnemyTurnSystem::calculateMoveOffset() const {
    constexpr float speed = 4.0f;
    sf::Vector2f moveOffset(0, 0);

    if (keysPressed.contains(sf::Keyboard::Scancode::Up) || 
        keysPressed.contains(sf::Keyboard::Scancode::W)) {
        moveOffset.y -= speed;
    }
    if (keysPressed.contains(sf::Keyboard::Scancode::Down) || 
        keysPressed.contains(sf::Keyboard::Scancode::S)) {
        moveOffset.y += speed;
    }
    if (keysPressed.contains(sf::Keyboard::Scancode::Left) || 
        keysPressed.contains(sf::Keyboard::Scancode::A)) {
        moveOffset.x -= speed;
    }
    if (keysPressed.contains(sf::Keyboard::Scancode::Right) || 
        keysPressed.contains(sf::Keyboard::Scancode::D)) {
        moveOffset.x += speed;
    }

    return moveOffset;
}

void EnemyTurnSystem::enforceBattleBoxBounds(sf::Vector2f& moveOffset) const {
    const BattleBox battleBox = ui.getBattleBox();
    const sf::FloatRect innerBounds = battleBox.getInnerBounds();

    const sf::Vector2f playerPos = player.getPosition() + moveOffset;
    const sf::Vector2f playerSize = {player.getGlobalBounds().size.x, player.getGlobalBounds().size.y};

    if (!innerBounds.findIntersection({playerPos, playerSize})) return;

    if (playerPos.x < innerBounds.position.x) {
        moveOffset.x = innerBounds.position.x - player.getPosition().x - battleBox.getOutlineThickness() + 1;
    }
    if (playerPos.y < innerBounds.position.y) {
        moveOffset.y = innerBounds.position.y - player.getPosition().y - battleBox.getOutlineThickness() + 1;
    }
    if ((playerPos.x + playerSize.x) > (innerBounds.position.x + innerBounds.size.x)) {
        moveOffset.x = (innerBounds.position.x + innerBounds.size.x) - (player.getPosition().x + playerSize.x) +
                       battleBox.getOutlineThickness() - 1;;
    }
    if ((playerPos.y + playerSize.y) > (innerBounds.position.y + innerBounds.size.y)) {
        moveOffset.y = (innerBounds.position.y + innerBounds.size.y) - (player.getPosition().y + playerSize.y) +
                       battleBox.getOutlineThickness() - 1;;
    }
}

void EnemyTurnSystem::processDamage() const {
    for (auto it = bulletManager.getBullets().begin(); it != bulletManager.getBullets().end(); ) {
        const Bullet* b = it->get();
        if (std::nullopt != player.getGlobalBounds().findIntersection(b->getGlobalBounds())) {
            if (!player.isHurting()) {
                ui.takePlayerDamage(4);
                it = bulletManager.getBullets().erase(it);
                if (ui.getCurrentHp() <= 0 && !deathManager.isActive()) {
                    deathManager.start();
                } else {
                    gameManager.triggerCameraShake(2);
                    player.startHurtAnimation();
                    gameManager.playSound("./sounds/snd_hurt1.wav");
                }
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }
}

bool EnemyTurnSystem::areBulletsActive() const {
    return !bulletManager.isEmpty();
}