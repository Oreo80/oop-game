#include "Headers/GameLogic.h"
#include <SFML/Audio.hpp>

int main() {
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


