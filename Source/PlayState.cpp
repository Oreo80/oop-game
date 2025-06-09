#include "../Headers/PlayState.h"
// #include "../Headers/GameOverState.h"
#include <random>
#include <numbers>

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

void PlayState::spawnShards(std::vector<std::unique_ptr<ShardEntity>> &shards, const sf::Vector2f &playerPos, int shardCount) {
    shards.clear();

    const std::vector<std::string> shardPaths = {
        "./img/spr_heartshards_0.png",
        "./img/spr_heartshards_1.png",
        "./img/spr_heartshards_2.png",
        "./img/spr_heartshards_3.png"
    };

    for (int i = 0; i < shardCount; ++i) {
        auto velocity = generateShardVelocity(i, shardCount);
        shards.push_back(std::make_unique<ShardEntity>(shardPaths, playerPos, velocity));
    }
}

static std::mt19937 rng(std::random_device{}());

sf::Vector2f PlayState::generateShardVelocity(const int index, const int totalShards) {
    const float angleDegrees = (static_cast<float>(index) / static_cast<float>(totalShards)) * 360.f;
    const float angleRadians = angleDegrees * std::numbers::pi_v<float> / 180.f;

    std::uniform_real_distribution speedDist(3.f, 6.f);
    const float speed = speedDist(rng);

    sf::Vector2f velocity;
    velocity.x = std::cos(angleRadians) * speed;
    velocity.y = -std::abs(std::sin(angleRadians) * speed); // upward initial burst

    return velocity;
}

void PlayState::updateDeath() {
    if (deathStage == DeathStage::None) return;

    deathFrame++;

    switch (deathFrame) {
        case 0:
            deathStage = DeathStage::ShowPlayer;
        break;
        case 30:
            deathStage = DeathStage::ShowBrokenHeart;
            gameManager.playSound("./sounds/snd_break1.wav");
        break;
        case 90:
            deathStage = DeathStage::ShowShards;
            gameManager.playSound("./sounds/snd_break2.wav");
            spawnShards(shards, player.getPosition(), 6);
        break;
        case 150:
            deathStage = DeathStage::FadeOut;
        break;
        default:
            break;
    }

}
void PlayState::renderDeath(sf::RenderWindow& window) {
    static sf::RectangleShape blackScreen;
    blackScreen.setSize({static_cast<float>(window.getSize().x) * 1.f, static_cast<float>(window.getSize().y) * 1.f});
    blackScreen.setFillColor(sf::Color::Black);
    window.draw(blackScreen);

    const auto playerPos = player.getPosition();

    switch (deathStage) {
        case DeathStage::ShowPlayer:
            player.tick(window);
        break;

        case DeathStage::ShowBrokenHeart:
        {
            static SpriteEntity brokenHeartSprite("./img/spr_heartbreak.png");
            brokenHeartSprite.setPosition(playerPos);
            brokenHeartSprite.tick(window);
        }
        break;

        case DeathStage::ShowShards:
            for (const auto& shard : shards)
                shard->tick(window);
        break;

        case DeathStage::FadeOut:
            gameManager.fadeOut(1.f);
        shouldTransition = true;
        break;

        default:
            break;
    }
}

void PlayState::doUpdate() {
    if (victoryAchieved) {
        victoryFrame++;
        if (victoryFrame > 60) {
            windowPtr->close();
        }
        return;
    }
    if (deathStage != DeathStage::None) {
        updateDeath();
        return;
    }
    switch (currentTurn) {
        case TurnState::PlayerTurn:
            updatePlayerTurn();
        break;
        case TurnState::EnemyTurn:
            updateEnemyTurn();
        break;
        case TurnState::SubMenu:
            updateSubMenu();
        break;
    }
}

void PlayState::doRender(sf::RenderWindow &window) {
    if (deathStage != DeathStage::None) {
        renderDeath(window);
        return;
    }
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
        &player,
        &hp,
        &froggit,
        &subMenuText[0],
        &subMenuText[1],
        &subMenuText[2],
        &subMenuText[3]
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

void PlayState::startDeath() {
    deathStage = DeathStage::ShowPlayer;
    deathFrame = 0;
    gameManager.stopMusic();
    shards.clear();
}

void PlayState::processDamage() {
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        if (const auto* b = dynamic_cast<Bullet*>(it->get())) {
            if (std::nullopt != player.getGlobalBounds().findIntersection(b->getGlobalBounds())) {
                if (!player.isHurting()) {
                    hp.takeDamage(4);
                    it = bullets.erase(it);
                    if (hp.getHp()<=0 && deathStage == DeathStage::None) {
                        startDeath();
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
    }

};


void PlayState::cleanupBullets() {
    std::erase_if(bullets, [&](auto& drawablePtr) {
        if (const auto* b = dynamic_cast<Bullet*>(drawablePtr.get())) {
            return b->isOffScreen(*windowPtr);
        }
        return false;
    });
}

void PlayState::enterPlayerTurn() {
    currentActionIndex = 0;
    const auto buttons = getButtons();
    player.setPosition(buttons[0]->getPositionForPlayer());
    buttons[0]->setSelected(true);
    keysPressed.clear();
    battleBox.resizeCentered({205 * 2, 0});
    battleText.setText(froggit.getFlavorText("Neutral"),0.5f);
}

void PlayState::updatePlayerTurn() {
    const auto buttons = getButtons();
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
        if (!battleBox.isUpdating()){
        buttons[currentActionIndex]->setSelected(false);
        processSelectedAction(currentActionIndex);
        // enterEnemyTurn();
        }
    }
}

void PlayState::enterEnemyTurn() {
    player.setState("normal");
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

    processDamage();
    cleanupBullets();

    if (!areBulletsActive() || enemyTurnClock.getElapsedTime().asSeconds() >= enemyTurnDuration) {
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
void PlayState::updateMenu(MenuState& menuState) {
    if (keysPressed.contains(sf::Keyboard::Scancode::Down)) {
        menuState.currentIndex = static_cast<int>((menuState.currentIndex + 1) % menuState.options.size());
        player.setPosition(subMenuText[menuState.currentIndex].getPositionForPlayer());
        keysPressed.erase(sf::Keyboard::Scancode::Down);
    }
    else if (keysPressed.contains(sf::Keyboard::Scancode::Up)) {
        menuState.currentIndex = static_cast<int>((menuState.currentIndex - 1 + menuState.options.size()) % menuState.options.size());
        player.setPosition(subMenuText[menuState.currentIndex].getPositionForPlayer());
        keysPressed.erase(sf::Keyboard::Scancode::Up);
    }

    if ((keysPressed.contains(sf::Keyboard::Scancode::Enter) ||
        (keysPressed.contains(sf::Keyboard::Scancode::Z))))
    {
        if (menuState.onSelect) {
            menuState.onSelect(menuState.currentIndex);
        }
        menuState.inMessagePhase = true;
        keysPressed.clear();
    }
}

void PlayState::enterSubMenu() {
    currentTurn = TurnState::SubMenu;
    player.centerPlayer(battleBox);
    keysPressed.clear();
    battleText.setText("");
}

void PlayState::exitSubMenu() {
    currentTurn = TurnState::PlayerTurn;
    currentSubMenu = SubMenuState::None;
    battleText.setText("");

    const auto buttons = getButtons();
    if (savedActionIndex < buttons.size()) {
        buttons[savedActionIndex]->setSelected(true);
        player.setPosition(buttons[savedActionIndex]->getPositionForPlayer());
    }
    keysPressed.clear();
    actionConfirmed = false;
    subMenuText[0].setColor(sf::Color::White);
}

void PlayState::updateMercyMenu() {
    if (keysPressed.contains(sf::Keyboard::Scancode::Enter) ||
        keysPressed.contains(sf::Keyboard::Scancode::Z)) {

        if (mercyConditionsMet) {
            battleText.setText("YOU WON!", 0.0f);
            subMenuText[0].setText("");
            player.setState("transparent");
            victoryAchieved = true;
            victoryFrame = 0;
        }
        actionConfirmed = true;
        }
}
void PlayState::useItem(const int itemIndex) {
    if (itemIndex < 0 || itemIndex >= static_cast<int>(inventory.size())) return;
    const Item usedItem = inventory[itemIndex];
    const int oldHp = hp.getHp();
    hp.heal(usedItem.healAmount);
    const int healed = hp.getHp() - oldHp;
    battleText.setText("* You used the " + usedItem.realName + ".\n* You recovered " +
                      std::to_string(healed) + " HP!", 0.0f);
    inventory.erase(inventory.begin() + itemIndex);
    for (auto& text : subMenuText) {
        text.setText("");
    }

    itemMessageDisplaying = true;
}

void PlayState::processActSelection(const int actIndex) {
    if (actIndex < 0 || actIndex >= static_cast<int>(actMenuState.options.size())) return;
    const auto& acts = froggit.getAvailableActs();
    const auto& selectedAct = acts[actIndex];
    froggit.increaseSpareProgress(selectedAct.spareProgress);
    battleText.setText(froggit.getFlavorText(selectedAct.name), 0.0f);
    for (auto& text : subMenuText) {
        text.setText("");
    }
    if (froggit.canBeSpared()) mercyConditionsMet = true;
    actFlavorTextDisplaying = true;

}

void PlayState::updateFightMenu() {
}

void PlayState::updateSubMenu() {
    if (keysPressed.contains(sf::Keyboard::Scancode::X) ||
        keysPressed.contains(sf::Keyboard::Scancode::LShift) ||
        keysPressed.contains(sf::Keyboard::Scancode::RShift)) {
        if (!actionConfirmed) {
            exitSubMenu();
        }
    }
    else if ((keysPressed.contains(sf::Keyboard::Scancode::Enter) ||
         keysPressed.contains(sf::Keyboard::Scancode::Z)) && actionConfirmed == true)
    {
        actionConfirmed = false;
        currentSubMenu = SubMenuState::None;
        enterEnemyTurn();
        keysPressed.clear();
        for (auto& text : subMenuText) {
            text.setText("");
        }
        return;
    }
    switch (currentSubMenu) {
        case SubMenuState::Fight:
            updateFightMenu();
        break;
        case SubMenuState::Talk:
            updateMenu(actMenuState);
        break;
        case SubMenuState::Item:
            updateMenu(itemMenuState);
        break;
        case SubMenuState::Spare:
            updateMercyMenu();
        break;
        default:
            battleText.setText(froggit.getFlavorText("Neutral"),0);
            for (auto& text : subMenuText) {
                text.setText("");
            }
            break;
    }
}
void PlayState::enterFightSubMenu() {
    // for (auto& text : subMenuText) {
    //     text.setText("");
    // }
    currentSubMenu = SubMenuState::Fight;
    const int damage = static_cast<int>(7 + rng()) % 5;
    froggit.takeDamage(damage);
    const std::string msg = "* You dealt " + std::to_string(damage) + " damage to " +
                     froggit.getName() + ". " + "\n" +
                     std::to_string(froggit.getCurrentHp()) + " HP remaining.";
    battleText.setText(msg,0);
    player.setState("transparent");
    actionConfirmed = true;
}

void PlayState::enterTalkSubMenu() {
    currentSubMenu = SubMenuState::Talk;

    actMenuState.options.clear();
    for (const auto& act : froggit.getAvailableActs()) {
        actMenuState.options.push_back("* " + act.name);
    }
    actMenuState.currentIndex = 0;
    actMenuState.inMessagePhase = false;
    actMenuState.onSelect = [this](const int index) {
        processActSelection(index);
        actMenuState.inMessagePhase = true;
        actionConfirmed = true;
        player.setState("transparent");
    };

    for (size_t i = 0; i < subMenuText.size(); i++) {
        if (i < actMenuState.options.size()) {
            subMenuText[i].setText(actMenuState.options[i]);
        } else {
            subMenuText[i].setText("");
        }
    }

    player.setPosition(subMenuText[0].getPositionForPlayer());
    actionConfirmed = false;
}

void PlayState::enterItemSubMenu() {
    currentSubMenu = SubMenuState::Item;

    itemMenuState.options.clear();
    for (const auto& item : inventory) {
        itemMenuState.options.push_back("* " + item.shortName);
    }
    itemMenuState.currentIndex = 0;
    itemMenuState.inMessagePhase = false;
    itemMenuState.onSelect = [this](int index) {
        useItem(index);
        itemMenuState.inMessagePhase = true;
        actionConfirmed = true;
        player.setState("transparent");
    };

    for (size_t i = 0; i < subMenuText.size(); i++) {
        if (i < itemMenuState.options.size()) {
            subMenuText[i].setText(itemMenuState.options[i]);
        } else {
            subMenuText[i].setText("");
        }
    }

    if (!inventory.empty()) {
        player.setPosition(subMenuText[0].getPositionForPlayer());
    }
    actionConfirmed = false;
}

void PlayState::enterMercySubMenu() {
    currentSubMenu = SubMenuState::Spare;
    for (auto& text : subMenuText) {
        text.setText("");
    }

    subMenuText[0].setText("* Spare");

    if (mercyConditionsMet) {
        subMenuText[0].setColor(sf::Color::Yellow);
    } else {
        subMenuText[0].setColor(sf::Color::White);
    }

    player.setPosition(subMenuText[0].getPositionForPlayer());
    actionConfirmed = false;
}

void PlayState::processSelectedAction(const int actionIndex) {
    enterSubMenu();
    switch (actionIndex) {
        case 0: // Fight
                enterFightSubMenu();
                std::cout << "Fight selected! Player attacks!\n";
        break;
        case 1: // Talk
                enterTalkSubMenu();
                std::cout << "Talk selected! Player tries to communicate.\n";
        break;
        case 2: // Item
                enterItemSubMenu();
                std::cout << "Item selected! Open inventory.\n";
        break;
        case 3: // Spare
            // battleText.setText("* You show mercy...");
                enterMercySubMenu();
                std::cout << "Spare selected! Attempting mercy...\n";
        break;
        default:
            std::cerr << "Invalid action index!\n";
    };
    savedActionIndex = actionIndex;
}

bool PlayState::areBulletsActive() const {
    return !bullets.empty();
}

void PlayState::print(std::ostream &os) const {
    GameState::print(os);
    os
        << "Play State, "
        << "Player: "<< player << ", Player HP: " << hp
        << ", Battle Box: "<<battleBox
        << ", Bullets Active: "<< (areBulletsActive() ? "Yes" : "No");
}

PlayState::PlayState() : background("./img/spr_battlebg_0.png"),
                         battleBox({242, 150},{155, 130}),
                         battleText("./fonts/fnt_main.png","./fonts/glyphs_fnt_main.csv",{52,270}) {
    gameManager.fadeIn(1.f);
    gameManager.playMusic("./mus/mus_battle1.ogg");
    battleBox.setBottomY(385.f);
    initEntities();
    enterPlayerTurn();
}

PlayState::PlayState(const PlayState &other)
    : GameState(other),
      background(other.background),
      shouldTransition(other.shouldTransition),
      player(other.player),
      battleBox(other.battleBox),
      keysPressed(other.keysPressed),
      battleText(other.battleText),
      hp(other.hp),
      froggit(other.froggit),
      fightButton(other.fightButton),
      talkButton(other.talkButton),
      itemButton(other.itemButton),
      spareButton(other.spareButton),
      currentTurn(other.currentTurn),
      currentActionIndex(other.currentActionIndex),
      waitingForTextDelay(other.waitingForTextDelay),
      deathStage(other.deathStage),
      deathFrame(other.deathFrame)
{
    bullets.reserve(other.bullets.size());
    for (const auto& proto : other.bullets) {
        bullets.push_back(proto->clone());
    }

    shards.reserve(other.shards.size());
    for (const auto& shard : other.shards) {
        shards.push_back(std::make_unique<ShardEntity>(*shard));
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

bool PlayState::shouldChangeState() const {
    return shouldTransition;
}

// std::unique_ptr<GameState> PlayState::nextState() {
//     return std::make_unique<GameOverState>();
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
    swap(first.hp, second.hp);
    swap(first.froggit, second.froggit);
    swap(first.fightButton, second.fightButton);
    swap(first.talkButton, second.talkButton);
    swap(first.itemButton, second.itemButton);
    swap(first.spareButton, second.spareButton);
    swap(first.currentTurn, second.currentTurn);
    swap(first.enemyTurnClock, second.enemyTurnClock);
    swap(first.currentActionIndex, second.currentActionIndex);
    swap(first.waitingForTextDelay, second.waitingForTextDelay);
    swap(first.deathStage, second.deathStage);
    swap(first.deathFrame, second.deathFrame);
    swap(first.shards, second.shards);

    first.initEntities();
    second.initEntities();
}

