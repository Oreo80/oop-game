#pragma once
#include <string>
#include <vector>
#include <iostream>

enum class BulletID { Fly, Temp };

class BulletType {
private:
    float speed;
    std::vector<std::string> paths;
    BulletType(float speed_, std::vector<std::string> paths_);

public:
    static const BulletType &get(BulletID id);
    [[nodiscard]] float getSpeed() const;
    [[nodiscard]] std::vector<std::string> getPaths() const;

    friend std::ostream & operator<<(std::ostream &os, const BulletType &obj);
};
