#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class BulletType {
private:
  std::vector<sf::Texture> textures;
  float speed;


public:
    explicit BulletType(const std::vector<std::string>& paths = {"./img/spr_flybullet_0.png", "./img/spr_flybullet_1.png"},
      float spd = 5.0f);

  friend std::ostream & operator<<(std::ostream &os, const BulletType &obj);

  [[nodiscard]] const std::vector<sf::Texture>& getTextures() const;
  [[nodiscard]] float getSpeed() const;

  BulletType(const BulletType &other);

  BulletType(BulletType &&other) noexcept;

  BulletType & operator=(const BulletType &other);
  BulletType & operator=(BulletType &&other) noexcept;
};