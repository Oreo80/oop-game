#pragma once
#include "BattleUISystem.h"
#include "Player.h"
#include <set>
#include <vector>
#include <string>

class PlayerTurnSystem {
public:
    enum class Signal {
        None,
        PlayerTurnComplete,
        VictoryAchieved
    };

private:
    BattleUISystem* ui;
    Player* player;

    struct Item {
        std::string realName;
        std::string shortName;
        int healAmount;
    };

    struct MenuState {
        std::vector<std::string> options;
        int currentIndex = 0;
        bool inMessagePhase = false;
        std::function<void(int)> onSelect;
    };

    enum class SubMenuState {
        None,
        Fight,
        Talk,
        Item,
        Spare
    };

    SubMenuState currentSubMenu = SubMenuState::None;
    MenuState actMenuState;
    MenuState itemMenuState;

    std::set<sf::Keyboard::Scancode>* keysPressed;
    bool actionConfirmed = false;
    bool mercyConditionsMet = false;
    size_t savedActionIndex = 0;
    int currentActionIndex = 0;

    std::vector<Item> inventory = {
        {"Monster Candy", "MnstrCndy", 10},
        {"Spider Donut", "SpidrDont", 12},
        {"Butterscotch Pie", "ButtsPie", 20},
        { "Spider Cider", "SpidrCidr", 18}
    };

    bool victorySequenceActive = false;
    int victoryFrame = 0;

    void processSelectedAction(int actionIndex);
    void enterSubMenu();
    void exitSubMenu();
    Signal updateSubMenu();
    void enterFightSubMenu();
    void enterTalkSubMenu();
    void enterItemSubMenu();
    void enterMercySubMenu();
    void useItem(int itemIndex);
    void processActSelection(int actIndex);
    Signal updateMenu(MenuState& menuState) const;
    Signal updateMercyMenu();
    void updateVictorySequence();

public:
    PlayerTurnSystem(BattleUISystem* ui, Player* player, std::set<sf::Keyboard::Scancode>* keysPressed);
    void start();
    Signal update();
};