#include "../Headers/DeathAnimationManager.h"
#include "../Headers/ShardEntity.h"
#include <random>
#include <numbers>

void DeathAnimationManager::spawnShards() {
    const std::vector<std::string> shardPaths = {
        "./img/spr_heartshards_0.png",
        "./img/spr_heartshards_1.png",
        "./img/spr_heartshards_2.png",
        "./img/spr_heartshards_3.png"
    };

    static std::mt19937 rng(std::random_device{}());
    const sf::Vector2f playerPos = player->getPosition();

    for (int i = 0; i < 6; ++i) {
        const float angle = (i / 6.0f) * 360.0f * (std::numbers::pi_v<float> / 180.0f);
        std::uniform_real_distribution speedDist(3.f, 6.f);
        const float speed = speedDist(rng);

        sf::Vector2f velocity(
            std::cos(angle) * speed,
            -std::abs(std::sin(angle) * speed) // upward initial burst
        );

        shards.push_back(std::make_unique<ShardEntity>(
            shardPaths,
            playerPos,
            velocity
        ));
    }
}
DeathAnimationManager::DeathAnimationManager(Player* player, GameManager* gameManager)
    : player(player), gameManager(gameManager) {}

void DeathAnimationManager::start() {
    currentStage = Stage::ShowPlayer;
    frameCounter = 0;
    shards.clear();
    gameManager->stopMusic();
}

void DeathAnimationManager::update() {
    if (!isActive()) return;

    frameCounter++;

    switch (currentStage) {
        case Stage::ShowPlayer:
            if (frameCounter >= 30) {
                currentStage = Stage::ShowBrokenHeart;
                gameManager->playSound("./sounds/snd_break1.wav");
            }
            break;
            
        case Stage::ShowBrokenHeart:
            if (frameCounter >= 90) {
                currentStage = Stage::ShowShards;
                spawnShards();
                gameManager->playSound("./sounds/snd_break2.wav");
            }
            break;
            
        case Stage::ShowShards:
            for (const auto& shard : shards) {
                shard->update();
            }
            if (frameCounter >= 150) {
                currentStage = Stage::FadeOut;
            }
            break;
            
        case Stage::FadeOut:
            gameManager->fadeOut(1.f);
            currentStage = Stage::None;
            break;
        default:
            break;
    }
}

void DeathAnimationManager::render(sf::RenderWindow& window) const {
    if (!isActive()) return;

    sf::RectangleShape blackScreen;
    blackScreen.setSize(static_cast<sf::Vector2f>(window.getSize()));
    blackScreen.setFillColor(sf::Color::Black);
    window.draw(blackScreen);

    const auto playerPos = player->getPosition();

    switch (currentStage) {
        case Stage::ShowPlayer:
            player->tick(window);
            break;
            
        case Stage::ShowBrokenHeart: {
            static SpriteEntity brokenHeartSprite("./img/spr_heartbreak.png");
            brokenHeartSprite.setPosition(playerPos);
            brokenHeartSprite.tick(window);
            break;
        }
            
        case Stage::ShowShards:
            for (const auto& shard : shards) {
                shard->tick(window);
            }
            break;
            
        default:
            break;
    }
}

bool DeathAnimationManager::isComplete() const {
    return currentStage == Stage::None;
}