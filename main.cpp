#include "./Headers/GameManager.h"
#include "./Headers/Exceptions.h"
#include <iostream>
int main() {
    try {
        auto& gm = GameManager::getInstance();
        gm.run();
    }
    catch (const std::exception& e) {
        std::cout << e.what() << '\n';
    }
    return 0;
}


