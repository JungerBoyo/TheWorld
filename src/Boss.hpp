#ifndef BOSSES_HPP
#define BOSSES_HPP

#include "Entity.hpp"
#include "AnimatedMovePattern.hpp"
#include <random>

struct Boss : public Entity
{
  Boss(glm::ivec2 pos, glm::ivec2 extent, int32_t dmgCooldown, int32_t health,
    std::string name,
    glm::ivec2 respawnLowerRestriction, glm::ivec2 respawnUpperRestriction,
    int32_t respawnRandomSeed,
    std::shared_ptr<Entity> target,
    std::string_view appearMvAnimJsonFilePath,
    std::string_view disappearMvAnimJsonFilePath,
    std::string_view targetMvAnimJsonFilePath
  );

  std::shared_ptr<const Bitmap> CurrentSprite() override;

  const auto& name() const { return name_; }

private:
  void Respawn();

private:
  std::string name_;

  glm::ivec2 moveVec_;
  std::mt19937 gen32_;

  std::shared_ptr<Entity> target_;

  AnimatedMovePattern appearMvAnim_;
  AnimatedMovePattern disappearMvAnim_;
  AnimatedMovePattern targetMvAnim_;

  enum class MoveType { appear, disappear, target };
  MoveType lastMoveType_;
};


#endif