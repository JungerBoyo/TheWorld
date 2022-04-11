#include "Boss.hpp"

Boss::Boss(glm::ivec2 pos, glm::ivec2 extent, int32_t dmgCooldown, int32_t health,
    std::string name,
    glm::ivec2 respawnLowerRestriction, glm::ivec2 respawnUpperRestriction,
    int32_t respawnRandomSeed,
    std::shared_ptr<Entity> target, 
    std::string_view appearMvAnimJsonFilePath,
    std::string_view disappearMvAnimJsonFilePath,
    std::string_view targetMvAnimJsonFilePath
  )
  : Entity(pos, extent, dmgCooldown, health),
    name_(std::move(name)),
    moveVec_((respawnUpperRestriction - respawnLowerRestriction)/2), //NOLINT scaling move vector a little
    gen32_{ static_cast<unsigned long>(respawnRandomSeed) },
    target_(std::move(target)),
    appearMvAnim_(appearMvAnimJsonFilePath),
    disappearMvAnim_(disappearMvAnimJsonFilePath),
    targetMvAnim_(targetMvAnimJsonFilePath),
    lastMoveType_(MoveType::appear)
{}

void Boss::Respawn()
{
  std::uniform_int_distribution<int32_t> x(-moveVec_[0], moveVec_[0]);
  std::uniform_int_distribution<int32_t> y(-moveVec_[1], moveVec_[1]);

  AccumulateNextMove({x(gen32_), y(gen32_)});
}

std::shared_ptr<const Bitmap> Boss::CurrentSprite()
{
  std::shared_ptr<const Bitmap> result;
  glm::ivec2 move;

  if(lastMoveType_ == MoveType::appear)
  {
    bool ended{false};
    std::tie(result, move) = appearMvAnim_.Iteration(ended);

    if(ended)
    {
      lastMoveType_ = MoveType::target;
    }
  }
  else if(lastMoveType_ == MoveType::target)
  { 
    bool ended{false};
    std::tie(result, move) = targetMvAnim_.Iteration(ended);

    if(ended)
    {
      LaunchSkill(target_->pos());
      lastMoveType_ = MoveType::disappear;
    }
  }
  else if(lastMoveType_ == MoveType::disappear)
  {
    bool ended{false};
    std::tie(result, move) = targetMvAnim_.Iteration(ended);

    if(ended)
    {
      Respawn();
      lastMoveType_ = MoveType::appear;
    }
  }

  return result;
}