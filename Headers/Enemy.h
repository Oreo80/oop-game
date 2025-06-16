#pragma once
#include "AnimatedEntity.h"
#include <unordered_map>
#include <vector>
#include <string>
struct EnemyAct {
    std::string name;
    int spareProgress;
};
class Enemy : public AnimatedEntity {
protected:
    std::string name;
    int maxHp = 0;
    int currentHp = 0;
    int attack = 0;
    int defense = 0;
    int spareThreshold = 0;
    int currentSpareProgress = 0;

    std::vector<EnemyAct> acts;

    std::unordered_map<std::string, std::vector<std::string>> flavorTexts;
    std::unordered_map<std::string, std::vector<std::string>> quotes;

    void loadFromJSON(const std::string& jsonPath);
    static std::string getRandomLine(const std::unordered_map<std::string, std::vector<std::string>>& source, const std::string& key);

public:
    Enemy(const std::vector<std::string>& texturePaths, const sf::Vector2f& pos, int animInterval, const std::string& jsonPath);

    std::string getFlavorText(const std::string& category) const;
    // std::string getQuote(const std::string& category) const;
    std::string getStatsText() const;
    std::vector<EnemyAct> getAvailableActs() const;
    std::string getName() const;
    [[nodiscard]] virtual std::unique_ptr<Enemy> cloneEnemy() const = 0;
    void takeDamage(int dmg);
    int getCurrentHp() const;
    int getAttack() const;
    int getDefense() const;
    int getSpareProgress() const;
    int getSpareThreshold() const;
    void increaseSpareProgress(int amount);
    bool canBeSpared() const;
};
