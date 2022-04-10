#include "Entity.hpp"

Entity::Entity(glm::ivec2 pos, glm::ivec2 extent, int32_t health)
  : pos_(pos),
    extent_(extent),
    fullHealth_(health),
    health_(health)
{}

bool Entity::InflictDmg(int32_t dmg, int32_t cooldown)
{
  health_ -= dmg;
  dmgCooldownCounter_ = cooldown;

  return health_ < 0;
}

void Entity::CommitNextPos([[maybe_unused]] glm::bvec2 xyUnchanged, glm::ivec2 nextPos)
{
  pos_ = nextPos;
  accumMv_ = {0, 0};
}

void Entity::AccumulateNextMove(glm::ivec2 mv)
{
  accumMv_ += mv;
}

void Entity::SetSkill(std::shared_ptr<Skill> skill) 
{
  skill_ = std::move(skill);   
}

void Entity::LaunchSkill(glm::ivec2 endPoint) 
{
  skill_->Launch(pos_ + (extent_/2), endPoint);// NOLINT    
}
