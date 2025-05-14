#include "../Headers/PlayState.h"
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
    switch (currentTurn) {
        case TurnState::PlayerTurn:
            updatePlayerTurn();
        break;
        case TurnState::EnemyTurn:
            updateEnemyTurn();
        break;
    }
}

void PlayState::doRender(sf::RenderWindow &window) const {
    for (auto* e : entities) {
        e->tick(window);
    }
    for (const auto& b : bullets) {
        b->tick(window);
    }
}

void PlayState::initEntities() {
    entities = {
        &background,
        &battleBox,
        &battleText,
        &fightButton,
        &talkButton,
        &itemButton,
        &spareButton,
        &player
    };
}

std::vector<Button *> PlayState::getButtons() const {
    std::vector<Button*> buttons;
    for (auto* e : entities) {
        if (auto* btn = dynamic_cast<Button*>(e)) {
            buttons.push_back(btn);
        }
    }
    return buttons;
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

void PlayState::enforceBattleBoxBounds(sf::Vector2f &moveOffset) const {
    const sf::FloatRect innerBounds=battleBox.getInnerBounds();

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

void PlayState::cleanupBullets() {
    std::erase_if(bullets, [this](const std::unique_ptr<Bullet>& bulletobj) {
    return bulletobj->isOffScreen(*windowPtr);
});

    std::erase_if(bullets, [this](const std::unique_ptr<Bullet>& bulletobj) {
        return std::nullopt != player.getGlobalBounds().findIntersection(bulletobj->getGlobalBounds());
    });
}

void PlayState::enterPlayerTurn() {
    currentActionIndex = 0;
    const auto buttons = getButtons();
    player.setPosition(buttons[0]->getPositionForPlayer());
    buttons[0]->setSelected(true);
    keysPressed.clear();
    battleBox.resizeCentered({205 * 2, 0});
    battleText.setText("* Smells like frog..?\n  Temporary text",0.5f);
}

void PlayState::updatePlayerTurn() {
    auto buttons = getButtons();
    if (keysPressed.contains(sf::Keyboard::Scancode::Left) || keysPressed.contains(sf::Keyboard::Scancode::A)) {
        currentActionIndex = (currentActionIndex - 1 + static_cast<int>(buttons.size())) % static_cast<int>(
                                 buttons.size());
        player.setPosition(buttons[currentActionIndex]->getPositionForPlayer());
        keysPressed.erase(sf::Keyboard::Scancode::Left);
        keysPressed.erase(sf::Keyboard::Scancode::A);
        updateButtonTextures();
    }
    if (keysPressed.contains(sf::Keyboard::Scancode::Right) || keysPressed.contains(sf::Keyboard::Scancode::D)) {
        currentActionIndex = (currentActionIndex + 1 + static_cast<int>(buttons.size())) % static_cast<int>(
                                 buttons.size());
        player.setPosition(buttons[currentActionIndex]->getPositionForPlayer());
        keysPressed.erase(sf::Keyboard::Scancode::Right);
        keysPressed.erase(sf::Keyboard::Scancode::D);
        updateButtonTextures();
    }

    if (keysPressed.contains(sf::Keyboard::Scancode::Enter) || keysPressed.contains(sf::Keyboard::Scancode::Z)) {
        processSelectedAction(currentActionIndex);
        buttons[currentActionIndex]->setSelected(false);
        enterEnemyTurn();
    }
}

void PlayState::enterEnemyTurn() {
    battleText.setText("");
    currentTurn = TurnState::EnemyTurn;
    keysPressed.clear();
    player.centerPlayer(battleBox);
    enemyTurnClock.restart();
    battleBox.resizeCentered({-205 * 2, 0});

    for (int i = 0; i < 5; i++) {
        bullets.emplace_back(std::make_unique<Bullet>(
            BulletID::Fly,
            sf::Vector2f{100.f, static_cast<float>(225 + (i + 1) * 30)},
            sf::Vector2f{1.0f, 0.0f}
        ));
    }
}

void PlayState::updateEnemyTurn() {
    sf::Vector2f moveOffset = calculateMoveOffset();
    enforceBattleBoxBounds(moveOffset);
    player.move(moveOffset);

    cleanupBullets();

    if (!isBulletsActive() || enemyTurnClock.getElapsedTime().asSeconds() >= enemyTurnDuration) {
        bullets.clear();
        currentTurn = TurnState::PlayerTurn;
        enterPlayerTurn();

    }
}

void PlayState::updateButtonTextures() const {
    int idx = 0;
    for (auto* e : entities) {
        if (auto* btn = dynamic_cast<Button*>(e)) {
            btn->setSelected(idx == currentActionIndex);
            ++idx;
        }
    }
}

void PlayState::processSelectedAction(const int actionIndex) {
    switch (actionIndex) {
        case 0: // Fight
            // battleText.setText("* You throw a punch!");
                std::cout << "Fight selected! Player attacks!\n";
        break;
        case 1: // Talk
            // battleText.setText("* You try to reason with them.");
                std::cout << "Talk selected! Player tries to communicate.\n";
        break;
        case 2: // Item
            // battleText.setText("* You check your inventory.");
                std::cout << "Item selected! Open inventory.\n";
        break;
        case 3: // Spare
            // battleText.setText("* You show mercy...");
                std::cout << "Spare selected! Attempting mercy...\n";
        break;
        default:
            std::cerr << "Invalid action index!\n";
    };
    waitingForTextDelay = true;
}

bool PlayState::isBulletsActive() const {
    return !bullets.empty();
}

PlayState::PlayState() : background("./img/spr_battlebg_0.png"),
battleBox({242, 150},{155, 130}),battleText("./fonts/fnt_main.png",
    "./fonts/glyphs_fnt_main.csv",{52,270}) {
    gameManager.playMusic("./mus/mus_battle1.ogg");
    battleBox.setBottomY(385.f);
    initEntities();
    enterPlayerTurn();
}

PlayState::PlayState(const PlayState &other)
    : GameState(other),
      background(dynamic_cast<const SpriteEntity&>(*other.background.clone())),
      shouldTransition(other.shouldTransition),
      player(dynamic_cast<const Player&>(*other.player.clone())),
      battleBox(dynamic_cast<const BattleBox&>(*other.battleBox.clone())),
      keysPressed(other.keysPressed),
      battleText(dynamic_cast<const BattleText&>(*other.battleText.clone())),
      fightButton(dynamic_cast<const Button&>(*other.fightButton.clone())),
      talkButton(dynamic_cast<const Button&>(*other.talkButton.clone())),
      itemButton(dynamic_cast<const Button&>(*other.itemButton.clone())),
      spareButton(dynamic_cast<const Button&>(*other.spareButton.clone())),
      currentTurn(other.currentTurn),
      currentActionIndex(other.currentActionIndex),
      waitingForTextDelay(other.waitingForTextDelay) {
    for (const auto& bullet : other.bullets) {
        bullets.push_back(std::unique_ptr<Bullet>(
            dynamic_cast<Bullet*>(bullet->clone().release())
        ));
    }
    initEntities();
}

PlayState & PlayState::operator=(PlayState other) {
    swap(*this, other);
    return *this;
}

// std::unique_ptr<GameState> PlayState::clone() const {
//     return std::make_unique<PlayState>(*this);
// }

// bool PlayState::shouldChangeState() const {
//     return false;
// }

void swap(PlayState &first, PlayState &second) noexcept {
    using std::swap;
    swap(first.background, second.background);
    swap(first.shouldTransition, second.shouldTransition);
    swap(first.player, second.player);
    swap(first.bullets, second.bullets);
    swap(first.battleBox, second.battleBox);
    swap(first.keysPressed, second.keysPressed);
    swap(first.battleText, second.battleText);
    swap(first.fightButton, second.fightButton);
    swap(first.talkButton, second.talkButton);
    swap(first.itemButton, second.itemButton);
    swap(first.spareButton, second.spareButton);
    swap(first.currentTurn, second.currentTurn);
    swap(first.enemyTurnClock, second.enemyTurnClock);
    swap(first.currentActionIndex, second.currentActionIndex);
    swap(first.waitingForTextDelay, second.waitingForTextDelay);

    first.initEntities();
    second.initEntities();
}
