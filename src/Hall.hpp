#ifndef HALL_HPP
#define HALL_HPP

#include "Entity.hpp"
#include "Boss.hpp"
#include "Bitmap.hpp"

struct Hall
{
  Hall() = default;
  Hall(
    std::string_view hallConfigPath, 
    glm::ivec2 bossPosition,
    glm::ivec2 respawnLowerRestriction, glm::ivec2 respawnUpperRestriction,
    const std::shared_ptr<Entity>& player);

  bool active{false};
  std::shared_ptr<Boss> boss;
  std::shared_ptr<Bitmap> map;
};

#endif