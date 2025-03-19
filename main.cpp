#include "Headers/GameLogic.h"
#include <SFML/Audio.hpp>

int main() {
    // BulletType flybullet({ "./img/spr_flybullet_0.png", "./img/spr_flybullet_1.png" }, 5.0f, { 1.0f, 0.0f });
    // Bullet bullet(flybullet, sf::Vector2f{100.f, 100.f});
    // Bullet A,B,C,D,E,F,G,H;
    // A=B=C=D=E=F=G=H=bullet;
    // D=std::move(F);
    // Bullet EE(std::move(D));
    try {
        Game game;
        game.run();
        std::cout << "Game exit status: " << game << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
    std::cout << "Programul a terminat execuția\n";
    return 0;
}


