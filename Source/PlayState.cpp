#include "../Headers/PlayState.h"
// #include "../Headers/GameOverState.h"
#include <random>

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
                enemyTurnSystem.start();
            currentTurn = TurnState::EnemyTurn;
            break;
            case PlayerTurnSystem::Signal::VictoryAchieved:
                windowPtr->close();
            break;
            default:
                break;
        }
    }
    else if (currentTurn == TurnState::EnemyTurn) {
        if (enemyTurnSystem.update()) { //return true when enemy turn is complete
            bulletManager.clearBullets();
            currentTurn = TurnState::PlayerTurn;
            playerTurnSystem.start();
        }
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

void PlayState::print(std::ostream &os) const {
    GameState::print(os);
    os
        << "Play State, "
        << "Player: "<< player;
}

PlayState::PlayState() : deathManager(&player, &gameManager),
                         playerTurnSystem(&ui, &player, &keysPressed, &gameManager),
                         enemyTurnSystem(player,ui,bulletManager,deathManager,keysPressed, gameManager){
    gameManager.fadeIn(1.f);
    gameManager.playMusic("./mus/mus_battle1.ogg");
    playerTurnSystem.start();
    const auto useless = player.clone();
    std::cout<<*useless;
    // total garbage, i know, n-am vrut sa comentez in 7 locuri diferite o functie care va trebui sa o folosesc
}

bool PlayState::shouldChangeState() const {
    return shouldTransition;
}