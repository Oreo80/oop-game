#include "../Headers/Enemy.h"
#include <random>
#include "../Headers/Exceptions.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

Enemy::Enemy(const std::vector<std::string>& texturePaths, const sf::Vector2f& pos, const int animInterval_, const std::string& jsonPath)
    : AnimatedEntity(texturePaths, pos, animInterval_) {
    loadFromJSON(jsonPath);
}
void Enemy::loadFromJSON(const std::string &jsonPath) {
    std::ifstream file(jsonPath);
    if (!file.is_open()) {
        throw JSONLoadException(jsonPath);
    }

    json data;
    file >> data;

    name = data.value("name", "Unknown Enemy");
    maxHp = data.value("hp", 0);
    currentHp = maxHp;
    attack = data.value("attack", 0);
    defense = data.value("defense", 0);
    spareThreshold = data.value("spareThreshold", 0);
    currentSpareProgress = 0;

    for (const auto& act : data["acts"]) {
        acts.push_back({
            act["name"].get<std::string>(),
            act["spareProgress"].get<int>()
        });
    }

    flavorTexts = data["flavorText"].get<std::unordered_map<std::string, std::vector<std::string>>>();
    quotes = data["quotes"].get<std::unordered_map<std::string, std::vector<std::string>>>();
}

std::string Enemy::getFlavorText(const std::string& category) const {
    if (category == "Neutral") {
        if (currentHp < 10) return getRandomLine(flavorTexts, "Low HP");
        if (canBeSpared()) return getRandomLine(flavorTexts, "Spare");
    }
    else if (category == "Check") return getStatsText();
    return getRandomLine(flavorTexts, category);
}
//
// std::string Enemy::getQuote(const std::string& category) const {
//     return getRandomLine(quotes, category);
// }

std::string Enemy::getStatsText() const {
    return "* " + name + " - ATK " + std::to_string(attack) + " DEF " + std::to_string(defense) + "\n" +
        getRandomLine(flavorTexts, "Check");
}

std::vector<EnemyAct> Enemy::getAvailableActs() const {
    return acts;
}
std::string Enemy::getName() const {
    return name;
}

void Enemy::takeDamage(const int dmg) {
    currentHp-=dmg;
}

int Enemy::getCurrentHp() const { return currentHp; }
int Enemy::getAttack() const { return attack; }
int Enemy::getDefense() const { return defense; }

int Enemy::getSpareProgress() const {
    return currentSpareProgress;
}

int Enemy::getSpareThreshold() const {
    return spareThreshold;
}

void Enemy::increaseSpareProgress(const int amount) {
    currentSpareProgress = std::min(currentSpareProgress + amount, spareThreshold);
}

bool Enemy::canBeSpared() const {
    return currentSpareProgress >= spareThreshold || currentHp < 10;
}

std::string Enemy::getRandomLine(const std::unordered_map<std::string, std::vector<std::string>>& source, const std::string& key) {
    const auto it = source.find(key);
    if (it == source.end() || it->second.empty()) return "[NO TEXT]";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, static_cast<int>(it->second.size()) - 1);
    return it->second[dis(gen)];
}
