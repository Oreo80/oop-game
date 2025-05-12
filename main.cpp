#include "./Headers/GameManager.h"
#include <iostream>
int main() {
    auto& gm = GameManager::getInstance();
    gm.run();
    std::cout << "Programul a terminat execuția\n";
    return 0;
}


