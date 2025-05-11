#include "Headers/GameLogic.h"
#include "./Headers/GameManager.h"
#include <iostream>
int main() {
    // try {
    //     Game game;
    //     game.run();
    //     std::cout << "Game exit status: " << game << std::endl;
    // } catch (const std::exception& e) {
    //     std::cerr << "Error: " << e.what() << '\n';
    // }
    auto& gm = GameManager::getInstance();
    gm.run();
    std::cout << "Programul a terminat execuția\n";
    return 0;
}


