#include "../Headers/BulletType.h"
#include <map>
#include <stdexcept>

BulletType::BulletType(const float speed_, std::vector<std::string> paths_) : speed{speed_}, paths{std::move(paths_)} {
    if (paths.empty()) {
        throw std::runtime_error("BulletType needs at least one frame");
    }
}

const BulletType & BulletType::get(const BulletID id) {
    static const std::map<BulletID, BulletType> registry = {
        { BulletID::Fly, BulletType{5.f, {"./img/spr_flybullet_0.png", "./img/spr_flybullet_1.png"} } },
        { BulletID::Temp, BulletType{ 3.f, {"./img/missing.png", "./img/missing.png", "./img/missing.png"} } }
    };
    const auto i = registry.find(id);
    if (i == registry.end())
        throw std::out_of_range("Unknown BulletID");
    return i->second;
}

float BulletType::getSpeed() const {
    return speed;
}

const std::vector<std::string>& BulletType::getPaths() const{
        return paths;
    }

std::ostream & operator<<(std::ostream &os, const BulletType &obj){
    os << "BulletType( speed=" << obj.speed << ", paths=[";
    for (size_t i = 0; i < obj.paths.size(); i++) {
        os << "'" << obj.paths[i] << "'";
        if (i != obj.paths.size() - 1)
            os << ", ";
    }
    os << "])";
    return os;
}
