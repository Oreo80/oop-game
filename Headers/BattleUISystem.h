#pragma once
#include "Button.h"
#include "BitmapFont.h"
#include "BattleBox.h"
#include "BattleText.h"
#include "Hp.h"
#include "Froggit.h"
#include "SpriteEntity.h"
#include "Player.h"

class BattleUISystem {
private:
    SpriteEntity background;
    BattleBox battleBox;
    BattleText battleText;
    Hp hp;
    // Froggit froggit;
    std::unique_ptr<Enemy> enemy;
    std::vector<Button> actionButtons;
    std::vector<BitmapFont> subMenuTexts;
    std::vector<DrawableEntity*> allComponents;

    void initUI();
public:
    BattleUISystem();
    void resizeBattleBox(const sf::Vector2f& delta);
    BattleBox& getBattleBox();
    std::unique_ptr<DrawableEntity> getEnemy();
    int getCurrentHp() const;
    void setFlavorText(float delay = 0.5f);
    void setBattleText(const std::string& text, float delay = 0.5f);
    void takePlayerDamage(int damage);
    void executeFightAction(int damage);
    void executeUseItemAction(const std::string &itemName, int healAmount);
    void executeActSelection(const EnemyAct &selectedAct);
    void selectButton(size_t index);
    void deselectAllButtons();
    size_t getButtonCount() const;
    void setSubMenuText(size_t index, const std::string& text);
    void clearSubMenuTexts();
    bool canSpareEnemy() const;
    void positionPlayerAtButton(Player &player, size_t index) const;
    void positionPlayerAtSubMenu(Player &player, size_t index) const;
    void setSubMenuColor(size_t index, sf::Color color);
    std::vector<EnemyAct> getEnemyActs() const;
    const std::vector<BitmapFont>& getSubMenuTexts() const;
    void tick(sf::RenderWindow& window) const;

};