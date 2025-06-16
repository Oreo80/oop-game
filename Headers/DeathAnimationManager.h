#pragma once
#include "Player.h"
#include "ShardEntity.h"
#include <vector>
#include <memory>
#include "GameManager.h"
enum class Stage {
    None,
    ShowPlayer,
    ShowBrokenHeart,
    ShowShards,
    FadeOut
};
class DeathAnimationManager {
private:
    Player* player;
    GameManager* gameManager;
    Stage currentStage = Stage::None;
    int frameCounter = 0;
    std::vector<std::unique_ptr<ShardEntity>> shards;
    void spawnShards();
public:
    DeathAnimationManager(Player* player, GameManager* gameManager);
    bool isActive() const { return currentStage != Stage::None; }
    void start();
    void update();
    void render(sf::RenderWindow& window) const;
    bool isComplete() const;
};