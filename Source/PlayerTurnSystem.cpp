#include "../Headers/PlayerTurnSystem.h"
#include <random>
#include <iostream>

PlayerTurnSystem::PlayerTurnSystem(BattleUISystem* ui, Player* player, std::set<sf::Keyboard::Scancode>* keysPressed )
    : ui(ui), player(player), keysPressed(keysPressed) {}

void PlayerTurnSystem::enterPlayerTurn() {
    currentActionIndex = 0;
    currentSubMenu = SubMenuState::None;
    actionConfirmed = false;
    mercyConditionsMet = ui->canSpareEnemy();

    ui->positionPlayerAtButton(*player, 0);
    ui->selectButton(0);
    ui->resizeBattleBox({205 * 2, 0});
    ui->setFlavorText();
    keysPressed->clear();
}

PlayerTurnSystem::Signal PlayerTurnSystem::update() {
    if (victorySequenceActive) {
        updateVictorySequence();
        return (victoryFrame > 60) ? Signal::VictoryAchieved : Signal::None;
    }

    if (currentSubMenu != SubMenuState::None) {
        return updateSubMenu();
    }

    if (keysPressed->contains(sf::Keyboard::Scancode::Left) ||
        keysPressed->contains(sf::Keyboard::Scancode::A)) {
        currentActionIndex = (currentActionIndex - 1 + static_cast<int>(ui->getButtonCount())) %
                             static_cast<int>(ui->getButtonCount());
        ui->positionPlayerAtButton(*player, currentActionIndex);
        keysPressed->erase(sf::Keyboard::Scancode::Left);
        keysPressed->erase(sf::Keyboard::Scancode::A);
        ui->deselectAllButtons();
        ui->selectButton(currentActionIndex);
    }
    else if (keysPressed->contains(sf::Keyboard::Scancode::Right) ||
             keysPressed->contains(sf::Keyboard::Scancode::D)) {
        currentActionIndex = (currentActionIndex + 1) % static_cast<int>(ui->getButtonCount());
        ui->positionPlayerAtButton(*player, currentActionIndex);
        keysPressed->erase(sf::Keyboard::Scancode::Right);
        keysPressed->erase(sf::Keyboard::Scancode::D);
        ui->deselectAllButtons();
        ui->selectButton(currentActionIndex);
    }

    if ((keysPressed->contains(sf::Keyboard::Scancode::Enter) ||
        keysPressed->contains(sf::Keyboard::Scancode::Z)) &&
        !ui->getBattleBox().isUpdating()) {
        ui->deselectAllButtons();
        processSelectedAction(currentActionIndex);
    }

    return Signal::None;
}

void PlayerTurnSystem::processSelectedAction(const int actionIndex) {
    savedActionIndex = actionIndex;
    enterSubMenu();

    switch (actionIndex) {
        case 0: enterFightSubMenu(); break;
        case 1: enterTalkSubMenu(); break;
        case 2: enterItemSubMenu(); break;
        case 3: enterMercySubMenu(); break;
        default: break;
    }
}

void PlayerTurnSystem::enterSubMenu() {
    currentSubMenu = SubMenuState::None;
    player->centerPlayer(ui->getBattleBox());
    ui->setBattleText("");
    keysPressed->clear();
    actionConfirmed = false;
}

void PlayerTurnSystem::exitSubMenu() {
    ui->setBattleText("");
    ui->clearSubMenuTexts();

    if (savedActionIndex < ui->getButtonCount()) {
        ui->selectButton(savedActionIndex);
        ui->positionPlayerAtButton(*player, savedActionIndex);
    }

    keysPressed->clear();
    actionConfirmed = false;
    currentSubMenu = SubMenuState::None;
}

PlayerTurnSystem::Signal PlayerTurnSystem::updateSubMenu() {
    if (keysPressed->contains(sf::Keyboard::Scancode::X) ||
        keysPressed->contains(sf::Keyboard::Scancode::LShift) ||
        keysPressed->contains(sf::Keyboard::Scancode::RShift)) {
        if (!actionConfirmed) {
            exitSubMenu();
            return Signal::None;
        }
    }
    else if ((keysPressed->contains(sf::Keyboard::Scancode::Enter) ||
             keysPressed->contains(sf::Keyboard::Scancode::Z)) &&
             actionConfirmed) {
        actionConfirmed = false;
        currentSubMenu = SubMenuState::None;
        ui->clearSubMenuTexts();
        return Signal::PlayerTurnComplete;
    }

    switch (currentSubMenu) {
        case SubMenuState::Talk: return updateMenu(actMenuState);
        case SubMenuState::Item: return updateMenu(itemMenuState);
        case SubMenuState::Spare: return updateMercyMenu();
        default: break;
    }

    return Signal::None;
}

PlayerTurnSystem::Signal PlayerTurnSystem::updateMenu(MenuState& menuState) const {
    if (keysPressed->contains(sf::Keyboard::Scancode::Down)) {
        menuState.currentIndex = (menuState.currentIndex + 1) % menuState.options.size();
        ui->positionPlayerAtSubMenu(*player, menuState.currentIndex);
        keysPressed->erase(sf::Keyboard::Scancode::Down);
    }
    else if (keysPressed->contains(sf::Keyboard::Scancode::Up)) {
        menuState.currentIndex = (menuState.currentIndex - 1 + menuState.options.size()) %
                                 menuState.options.size();
        ui->positionPlayerAtSubMenu(*player, menuState.currentIndex);
        keysPressed->erase(sf::Keyboard::Scancode::Up);
    }

    if ((keysPressed->contains(sf::Keyboard::Scancode::Enter) ||
        keysPressed->contains(sf::Keyboard::Scancode::Z))) {
        if (menuState.onSelect) {
            menuState.onSelect(menuState.currentIndex);
        }
        menuState.inMessagePhase = true;
        keysPressed->clear();
    }

    return Signal::None;
}

PlayerTurnSystem::Signal PlayerTurnSystem::updateMercyMenu() {
    if ((keysPressed->contains(sf::Keyboard::Scancode::Enter) ||
        keysPressed->contains(sf::Keyboard::Scancode::Z))) {
        if (mercyConditionsMet) {
            ui->setBattleText("YOU WON", 0);
            ui->setSubMenuText(0, "");
            player->setState("transparent");
            victorySequenceActive = true;
            victoryFrame = 0;
        }
        actionConfirmed = true;
        keysPressed->clear();
    }
    return Signal::None;
}

void PlayerTurnSystem::enterFightSubMenu() {
    currentSubMenu = SubMenuState::Fight;

    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(3, 7);
    int damage = dist(rng);

    ui->executeFightAction(damage);
    player->setState("transparent");
    actionConfirmed = true;
}

void PlayerTurnSystem::enterTalkSubMenu() {
    currentSubMenu = SubMenuState::Talk;
    actMenuState = MenuState{};

    for (const auto& act : ui->getFroggitActs()) {
        actMenuState.options.push_back("* " + act.name);
    }

    actMenuState.currentIndex = 0;
    actMenuState.inMessagePhase = false;
    actMenuState.onSelect = [this](const int index) {
        this->processActSelection(index);
        actionConfirmed = true;
        player->setState("transparent");
    };

    for (size_t i = 0; i < ui->getSubMenuTexts().size(); i++) {
        ui->setSubMenuText(i, (i < actMenuState.options.size()) ?
                           actMenuState.options[i] : "");
    }

    ui->positionPlayerAtSubMenu(*player, 0);
    actionConfirmed = false;
}

void PlayerTurnSystem::enterItemSubMenu() {
    currentSubMenu = SubMenuState::Item;
    itemMenuState = MenuState{};

    for (const auto& item : inventory) {
        itemMenuState.options.push_back("* " + item.shortName);
    }

    itemMenuState.currentIndex = 0;
    itemMenuState.inMessagePhase = false;
    itemMenuState.onSelect = [this](const int index) {
        this->useItem(index);
        actionConfirmed = true;
        player->setState("transparent");
    };

    for (size_t i = 0; i < ui->getSubMenuTexts().size(); i++) {
        ui->setSubMenuText(i, (i < itemMenuState.options.size()) ?
                           itemMenuState.options[i] : "");
    }

    if (!inventory.empty()) {
        ui->positionPlayerAtSubMenu(*player, 0);
    }
    actionConfirmed = false;
}

void PlayerTurnSystem::enterMercySubMenu() {
    currentSubMenu = SubMenuState::Spare;
    ui->clearSubMenuTexts();
    ui->setSubMenuText(0, "* Spare");

    mercyConditionsMet = ui->canSpareEnemy();
    ui->setSubMenuColor(0, mercyConditionsMet ? sf::Color::Yellow : sf::Color::White);
    ui->positionPlayerAtSubMenu(*player, 0);
    actionConfirmed = false;
}

void PlayerTurnSystem::useItem(const int itemIndex) {
    if (itemIndex < 0 || itemIndex >= static_cast<int>(inventory.size())) return;

    const Item& usedItem = inventory[itemIndex];
    ui->executeUseItemAction(usedItem.realName, usedItem.healAmount);
    inventory.erase(inventory.begin() + itemIndex);
}

void PlayerTurnSystem::processActSelection(const int actIndex) {
    if (actIndex < 0 || actIndex >= static_cast<int>(actMenuState.options.size())) return;

    const auto& acts = ui->getFroggitActs();
    const auto& selectedAct = acts[actIndex];
    ui->executeActSelection(selectedAct);
    mercyConditionsMet = ui->canSpareEnemy();
}

void PlayerTurnSystem::updateVictorySequence() {
    victoryFrame++;
}