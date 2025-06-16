#include "../Headers/BattleUISystem.h"

BattleUISystem::BattleUISystem() :
    background("./img/spr_battlebg_0.png"),
    battleBox({242, 150}, {155, 130}),
    battleText("./fonts/fnt_main.png", "./fonts/glyphs_fnt_main.csv", {52, 270}),
    hp(20)
{
    enemy = std::make_unique<Froggit>();
    battleBox.setBottomY(385.f);
    initUI();
    allComponents = {
        &background,
        &battleBox,
        &battleText,
        &hp,
        &*enemy
    };

    for (auto& button : actionButtons) {
        allComponents.push_back(&button);
    }

    for (auto& text : subMenuTexts) {
        allComponents.push_back(&text);
    }
}

void BattleUISystem::initUI() {
    actionButtons = {
        Button("./img/spr_fightbt_0.png", "./img/spr_fightbt_1.png", {32, 432}),
        Button("./img/spr_talkbt_0.png", "./img/spr_talkbt_1.png", {185, 432}),
        Button("./img/spr_itembt_0.png", "./img/spr_itembt_1.png", {345, 432}),
        Button("./img/spr_sparebt_0.png", "./img/spr_sparebt_1.png", {500, 432})
    };

    subMenuTexts = {
        BitmapFont("./fonts/fnt_main.png", "./fonts/glyphs_fnt_main.csv", "", {90, 268}, sf::Color::White, 1.f),
        BitmapFont("./fonts/fnt_main.png", "./fonts/glyphs_fnt_main.csv", "", {350, 268}, sf::Color::White, 1.f),
        BitmapFont("./fonts/fnt_main.png", "./fonts/glyphs_fnt_main.csv", "", {90, 310}, sf::Color::White, 1.f),
        BitmapFont("./fonts/fnt_main.png", "./fonts/glyphs_fnt_main.csv", "", {350, 310}, sf::Color::White, 1.f)
    };
}

void BattleUISystem::resizeBattleBox(const sf::Vector2f& delta) {
    battleBox.resizeCentered(delta);
}

BattleBox & BattleUISystem::getBattleBox() {
    return battleBox;
}

std::unique_ptr<DrawableEntity> BattleUISystem::getEnemy() {
    return enemy->clone();
}

int BattleUISystem::getCurrentHp() const {
    return hp.getHp();
}

void BattleUISystem::setFlavorText(const float delay) {
    battleText.setText(enemy->getFlavorText("Neutral"), delay);
    // hopefully more logic soon
}

void BattleUISystem::setBattleText(const std::string& text, const float delay) {
    battleText.setText(text, delay);
}

void BattleUISystem::takePlayerDamage(const int damage) {
    hp.takeDamage(damage);
}

void BattleUISystem::executeFightAction(const int damage) {
    enemy->takeDamage(damage);
    const std::string msg = "* You dealt " + std::to_string(damage) + " damage to " +
                     enemy->getName() + ". " + "\n" +
                     std::to_string(enemy->getCurrentHp()) + " HP remaining.";
    battleText.setText(msg,0);
}

void BattleUISystem::executeUseItemAction(const std::string &itemName, const int healAmount) {
    const int oldHp = hp.getHp();
    hp.heal(healAmount);
    const int healed = hp.getHp() - oldHp;
    battleText.setText("* You used the " + itemName + ".\n* You recovered " +
                      std::to_string(healed) + " HP!", 0.0f);
    clearSubMenuTexts();
}

void BattleUISystem::executeActSelection(const EnemyAct &selectedAct) {
    enemy->increaseSpareProgress(selectedAct.spareProgress);
    battleText.setText(enemy->getFlavorText(selectedAct.name), 0.0f);
    clearSubMenuTexts();
}

void BattleUISystem::selectButton(const size_t index) {
    if (index < actionButtons.size()) {
        deselectAllButtons();
        actionButtons[index].setSelected(true);
    }
}

void BattleUISystem::deselectAllButtons() {
    for (auto& button : actionButtons) {
        button.setSelected(false);
    }
}

size_t BattleUISystem::getButtonCount() const {
    return actionButtons.size();
}

void BattleUISystem::setSubMenuText(const size_t index, const std::string& text) {
    if (index < subMenuTexts.size()) {
        subMenuTexts[index].setText(text);
    }
}

void BattleUISystem::clearSubMenuTexts() {
    for (auto& text : subMenuTexts) {
        text.setText("");
    }
}

bool BattleUISystem::canSpareEnemy() const {
    return enemy->canBeSpared();
}

void BattleUISystem::positionPlayerAtButton(Player &player, const size_t index) const {
    if (index < actionButtons.size()) {
        player.setPosition(actionButtons[index].getPositionForPlayer());
    }
}

void BattleUISystem::positionPlayerAtSubMenu(Player& player, const size_t index) const {
    if (index < subMenuTexts.size()) {
        player.setPosition(subMenuTexts[index].getPositionForPlayer());
    }
}

void BattleUISystem::setSubMenuColor(const size_t index, const sf::Color color) {
    subMenuTexts[index].setColor(color);
}

std::vector<EnemyAct> BattleUISystem::getEnemyActs() const {
    return enemy->getAvailableActs();
}

const std::vector<BitmapFont>& BattleUISystem::getSubMenuTexts() const {
    return subMenuTexts;
}

void BattleUISystem::tick(sf::RenderWindow &window) const {
    for (auto& c : allComponents) {
        c->tick(window);
    }
}
