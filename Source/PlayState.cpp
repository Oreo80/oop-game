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
    ui.tick(window);
    bulletManager.tick(window);
    for (auto* e : entities) {
        e->tick(window);
    }
}

void PlayState::initEntities() {
    entities = {
        &player
    };
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

void PlayState::startDeath() {
    deathStage = DeathStage::ShowPlayer;
    deathFrame = 0;
    gameManager.stopMusic();
    shards.clear();
}

void PlayState::processDamage() {
    for (auto it = bulletManager.getBullets().begin(); it != bulletManager.getBullets().end(); ) {
        const Bullet* b = it->get();
        if (std::nullopt != player.getGlobalBounds().findIntersection(b->getGlobalBounds())) {
            if (!player.isHurting()) {
                ui.takePlayerDamage(4);
                it = bulletManager.getBullets().erase(it);
                if (ui.getCurrentHp()<=0 && deathStage == DeathStage::None) {
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

};


// void PlayState::cleanupBullets() {
//     std::erase_if(bullets, [&](auto& drawablePtr) {
//         if (const auto* b = dynamic_cast<Bullet*>(drawablePtr.get())) {
//             return b->isOffScreen(*windowPtr);
//         }
//         return false;
//     });
// }

void PlayState::enterPlayerTurn() {
    currentActionIndex = 0;
    ui.positionPlayerAtButton(player,0);
    ui.selectButton(0);
    keysPressed.clear();
    ui.resizeBattleBox({205 * 2, 0});
    ui.setFlavorText();
}

void PlayState::updatePlayerTurn() {
    if (keysPressed.contains(sf::Keyboard::Scancode::Left) || keysPressed.contains(sf::Keyboard::Scancode::A)) {
        currentActionIndex = (currentActionIndex - 1 + static_cast<int>(ui.getButtonCount())) % static_cast<int>(
                                 ui.getButtonCount());
        ui.positionPlayerAtButton(player, currentActionIndex);
        keysPressed.erase(sf::Keyboard::Scancode::Left);
        keysPressed.erase(sf::Keyboard::Scancode::A);
        ui.deselectAllButtons();
        ui.selectButton(currentActionIndex);
    }
    if (keysPressed.contains(sf::Keyboard::Scancode::Right) || keysPressed.contains(sf::Keyboard::Scancode::D)) {
        currentActionIndex = (currentActionIndex + 1 + static_cast<int>(ui.getButtonCount())) % static_cast<int>(
                                 ui.getButtonCount());
        ui.positionPlayerAtButton(player, currentActionIndex);
        keysPressed.erase(sf::Keyboard::Scancode::Right);
        keysPressed.erase(sf::Keyboard::Scancode::D);
        ui.deselectAllButtons();
        ui.selectButton(currentActionIndex);
    }

    if (keysPressed.contains(sf::Keyboard::Scancode::Enter) || keysPressed.contains(sf::Keyboard::Scancode::Z)) {
        if (!ui.getBattleBox().isUpdating()){
        ui.deselectAllButtons();
        processSelectedAction(currentActionIndex);
        }
    }
}

void PlayState::enterEnemyTurn() {
    player.setState("normal");
    ui.setBattleText("");
    currentTurn = TurnState::EnemyTurn;
    keysPressed.clear();
    player.centerPlayer(ui.getBattleBox());
    enemyTurnClock.restart();
    ui.resizeBattleBox({-205 * 2, 0});

    // for (int i = 0; i < 5; i++) {
    //     bullets.emplace_back(std::make_unique<Bullet>(
    //         BulletID::Fly,
    //         sf::Vector2f{100.f, static_cast<float>(225 + (i + 1) * 30)},
    //         sf::Vector2f{1.0f, 0.0f}
    //     ));
    // }
    bulletManager.spawnFlyBullets(5,225,30, {1.0f, 0.f});
}

void PlayState::updateEnemyTurn() {
    sf::Vector2f moveOffset = calculateMoveOffset();
    enforceBattleBoxBounds(moveOffset);
    player.move(moveOffset);

    processDamage();
    // cleanupBullets();

    if (!areBulletsActive() || enemyTurnClock.getElapsedTime().asSeconds() >= enemyTurnDuration) {
        // bullets.clear();
        bulletManager.clearBullets();
        currentTurn = TurnState::PlayerTurn;
        enterPlayerTurn();

    }
}

void PlayState::updateMenu(MenuState& menuState) {
    if (keysPressed.contains(sf::Keyboard::Scancode::Down)) {
        menuState.currentIndex = static_cast<int>((menuState.currentIndex + 1) % menuState.options.size());
        ui.positionPlayerAtSubMenu(player, menuState.currentIndex);
        keysPressed.erase(sf::Keyboard::Scancode::Down);
    }
    else if (keysPressed.contains(sf::Keyboard::Scancode::Up)) {
        menuState.currentIndex = static_cast<int>((menuState.currentIndex - 1 + menuState.options.size()) % menuState.options.size());
        ui.positionPlayerAtSubMenu(player, menuState.currentIndex);
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
    player.centerPlayer(ui.getBattleBox());
    keysPressed.clear();
    ui.setBattleText("");
}

void PlayState::exitSubMenu() {
    currentTurn = TurnState::PlayerTurn;
    currentSubMenu = SubMenuState::None;
    ui.setBattleText("");

    if (savedActionIndex < ui.getButtonCount()) {
        ui.selectButton(savedActionIndex);
        ui.positionPlayerAtButton(player, savedActionIndex);
    }
    keysPressed.clear();
    actionConfirmed = false;
    ui.setSubMenuColor(1,sf::Color::White);
}

void PlayState::updateMercyMenu() {
    if (keysPressed.contains(sf::Keyboard::Scancode::Enter) ||
        keysPressed.contains(sf::Keyboard::Scancode::Z)) {

        if (mercyConditionsMet) {
            ui.setBattleText("YOU WON", 0);
            ui.setSubMenuText(0,"");
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
    ui.executeUseItemAction(usedItem.realName, usedItem.healAmount);
    inventory.erase(inventory.begin() + itemIndex);
    itemMenuState.inMessagePhase = true;
}

void PlayState::processActSelection(const int actIndex) {
    if (actIndex < 0 || actIndex >= static_cast<int>(actMenuState.options.size())) return;
    const auto& acts = ui.getFroggitActs();
    const auto& selectedAct = acts[actIndex];
    ui.executeActSelection(selectedAct);
    if (ui.canSpareEnemy()) mercyConditionsMet = true;
    actMenuState.inMessagePhase = true;
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
        ui.clearSubMenuTexts();
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
            ui.setFlavorText(0);
            ui.clearSubMenuTexts();
            break;
    }
}
void PlayState::enterFightSubMenu() {
    currentSubMenu = SubMenuState::Fight;
    const int damage = static_cast<int>(7 + rng()) % 5;
    ui.executeFightAction(damage);
    player.setState("transparent");
    actionConfirmed = true;
}

void PlayState::enterTalkSubMenu() {
    currentSubMenu = SubMenuState::Talk;

    actMenuState.options.clear();
    for (const auto& act : ui.getFroggitActs()) {
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

    for (size_t i = 0; i < ui.getSubMenuTexts().size(); i++) {
        if (i < actMenuState.options.size()) {
            ui.setSubMenuText(i,actMenuState.options[i]);
        } else {
            ui.setSubMenuText(i,"");
        }
    }

    ui.positionPlayerAtSubMenu(player,0);
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

    for (size_t i = 0; i < ui.getSubMenuTexts().size(); i++) {
        if (i < itemMenuState.options.size()) {
            ui.setSubMenuText(i,itemMenuState.options[i]);
        } else {
            ui.setSubMenuText(i,"");

        }
    }

    if (!inventory.empty()) {
        ui.positionPlayerAtSubMenu(player,0);
    }
    actionConfirmed = false;
}

void PlayState::enterMercySubMenu() {
    currentSubMenu = SubMenuState::Spare;
    ui.clearSubMenuTexts();
    ui.setSubMenuText(0,"* Spare");

    if (mercyConditionsMet) {
        ui.setSubMenuColor(0,sf::Color::Yellow);
    } else {
        ui.setSubMenuColor(0,sf::Color::White);
    }
    ui.positionPlayerAtSubMenu(player,0);
    actionConfirmed = false;
}

void PlayState::processSelectedAction(const int actionIndex) {
    enterSubMenu();
    switch (actionIndex) {
        case 0: // Fight
                enterFightSubMenu();
                // std::cout << "Fight selected! Player attacks!\n";
        break;
        case 1: // Talk
                enterTalkSubMenu();
                // std::cout << "Talk selected! Player tries to communicate.\n";
        break;
        case 2: // Item
                enterItemSubMenu();
                // std::cout << "Item selected! Open inventory.\n";
        break;
        case 3: // Spare
            // battleText.setText("* You show mercy...");
                enterMercySubMenu();
                // std::cout << "Spare selected! Attempting mercy...\n";
        break;
        default:
            std::cerr << "Invalid action index!\n";
    };
    savedActionIndex = actionIndex;
}

bool PlayState::areBulletsActive() const {
    // return !bullets.empty();
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

PlayState::PlayState() {
    gameManager.fadeIn(1.f);
    gameManager.playMusic("./mus/mus_battle1.ogg");
    initEntities();
    enterPlayerTurn();
}

PlayState::PlayState(const PlayState &other)
    : GameState(other),
      shouldTransition(other.shouldTransition),
      player(other.player),
      keysPressed(other.keysPressed),
      currentTurn(other.currentTurn),
      currentActionIndex(other.currentActionIndex),
      waitingForTextDelay(other.waitingForTextDelay),
      deathStage(other.deathStage),
      deathFrame(other.deathFrame)
{
    // bullets.reserve(other.bullets.size());
    // for (const auto& proto : other.bullets) {
    //     bullets.push_back(proto->clone());
    // }

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
    swap(first.shouldTransition, second.shouldTransition);
    swap(first.player, second.player);
    // swap(first.bullets, second.bullets);
    swap(first.keysPressed, second.keysPressed);
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

