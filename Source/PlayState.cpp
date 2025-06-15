#include "../Headers/PlayState.h"
// #include "../Headers/GameOverState.h"
#include <random>

const float PlayState::enemyTurnDuration = 10.f;
void PlayState::doProcessEvent(const std::optional<sf::Event> &event) {
    if (event->is<sf::Event::KeyPressed>()) {
        const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();
        keysPressed.insert(keyPressed->scancode);
    }
    if (event->is<sf::Event::KeyReleased>()) {
        const auto* keyReleased = event->getIf<sf::Event::KeyReleased>();
        keysPressed.erase(keyReleased->scancode);
    }
}

void PlayState::doUpdate() {
    if (deathManager.isActive()) {
        deathManager.update();
        if (deathManager.isComplete()) {
            shouldTransition = true;
        }
        return;
    }
    if (currentTurn == TurnState::PlayerTurn) {
        switch (playerTurnSystem.update()) {
            case PlayerTurnSystem::Signal::PlayerTurnComplete:
                enterEnemyTurn();
            break;
            case PlayerTurnSystem::Signal::VictoryAchieved:
                windowPtr->close();
            break;
            default:
                break;
        }
    }
    else if (currentTurn == TurnState::EnemyTurn) {
        updateEnemyTurn();
    }
}

void PlayState::doRender(sf::RenderWindow &window) {
    if (deathManager.isActive()) {
        deathManager.render(window);
        return;
    }
    deathManager.render(window);
    ui.tick(window);
    bulletManager.tick(window);
    player.tick(window);
}

sf::Vector2f PlayState::calculateMoveOffset() const {
    constexpr float speed = 4.0f;
    sf::Vector2f moveOffset(0, 0);

    if (keysPressed.contains(sf::Keyboard::Scancode::Up) || keysPressed.contains(sf::Keyboard::Scancode::W)) {
        moveOffset.y -= speed;
    }
    if (keysPressed.contains(sf::Keyboard::Scancode::Down) || keysPressed.contains(sf::Keyboard::Scancode::S)) {
        moveOffset.y += speed;
    }
    if (keysPressed.contains(sf::Keyboard::Scancode::Left) || keysPressed.contains(sf::Keyboard::Scancode::A)){
        moveOffset.x -= speed;
    }
    if (keysPressed.contains(sf::Keyboard::Scancode::Right) || keysPressed.contains(sf::Keyboard::Scancode::D)){
        moveOffset.x += speed;
    }

    return moveOffset;
}

void PlayState::enforceBattleBoxBounds(sf::Vector2f &moveOffset) {
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

void PlayState::processDamage() {
    for (auto it = bulletManager.getBullets().begin(); it != bulletManager.getBullets().end(); ) {
        const Bullet* b = it->get();
        if (std::nullopt != player.getGlobalBounds().findIntersection(b->getGlobalBounds())) {
            if (!player.isHurting()) {
                ui.takePlayerDamage(4);
                it = bulletManager.getBullets().erase(it);
                if (ui.getCurrentHp()<=0 && !deathManager.isActive()) {
                    deathManager.start();
                }
                else {
                    gameManager.triggerCameraShake(2);
                    player.startHurtAnimation();
                    gameManager.playSound("./sounds/snd_hurt1.wav");
                }
            }
            else {
                ++it; // player has invincibility if it already got hit
            }
        } else {
            ++it;
        }
    }

};
void PlayState::enterEnemyTurn() {
    player.setState("normal");
    ui.setBattleText("");
    currentTurn = TurnState::EnemyTurn;
    keysPressed.clear();
    player.centerPlayer(ui.getBattleBox());
    enemyTurnClock.restart();
    ui.resizeBattleBox({-205 * 2, 0});

    bulletManager.spawnFlyBullets(5,225,30, {1.0f, 0.f});
}

void PlayState::updateEnemyTurn() {
    sf::Vector2f moveOffset = calculateMoveOffset();
    enforceBattleBoxBounds(moveOffset);
    player.move(moveOffset);
    processDamage();
    if (!areBulletsActive() || enemyTurnClock.getElapsedTime().asSeconds() >= enemyTurnDuration) {
        bulletManager.clearBullets();
        currentTurn = TurnState::PlayerTurn;
        playerTurnSystem.enterPlayerTurn();
    }
}


bool PlayState::areBulletsActive() const {
    return !bulletManager.isEmpty();
}

void PlayState::print(std::ostream &os) const {
    GameState::print(os);
    os
        << "Play State, "
        << "Player: "<< player
        // << ", Player HP: " << hp
        // << ", Battle Box: "<<battleBox
        << ", Bullets Active: "<< (areBulletsActive() ? "Yes" : "No");
}

PlayState::PlayState() : deathManager(&player, &gameManager), playerTurnSystem(&ui, &player, &keysPressed) {
    gameManager.fadeIn(1.f);
    gameManager.playMusic("./mus/mus_battle1.ogg");
    playerTurnSystem.enterPlayerTurn();
    const auto useless = player.clone();
    std::cout<<*useless;
    // total garbage, i know, n-am vrut sa comentez in 7 locuri diferite o functie care va trebui sa o folosesc
}

PlayState::PlayState(const PlayState &other)
    : GameState(other),
      shouldTransition(other.shouldTransition),
      player(other.player),
      keysPressed(other.keysPressed),
      deathManager(&player, &gameManager),
      playerTurnSystem(&ui, &player, &keysPressed),
      currentTurn(other.currentTurn),
      waitingForTextDelay(other.waitingForTextDelay){
}

PlayState & PlayState::operator=(PlayState other) {
    swap(*this, other);
    return *this;
}

bool PlayState::shouldChangeState() const {
    return shouldTransition;
}

void swap(PlayState &first, PlayState &second) noexcept {
    using std::swap;
    swap(first.shouldTransition, second.shouldTransition);
    swap(first.player, second.player);
    swap(first.keysPressed, second.keysPressed);
    swap(first.currentTurn, second.currentTurn);
    swap(first.enemyTurnClock, second.enemyTurnClock);
    swap(first.waitingForTextDelay, second.waitingForTextDelay);
}

