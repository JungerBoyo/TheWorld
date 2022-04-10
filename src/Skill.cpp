#include "Skill.hpp"
#include <glm/glm.hpp>

Chien::Chien()
  : Skill(0, 0, 1, 100, Type::lines)// NOLINT
{}

std::pair<glm::ivec2, glm::ivec2> Chien::Iteration()
{
  if(this->cooldownCounter_ > 0)
  {
    --this->cooldownCounter_;
  }
    
  if(launchedCounter_ == 0)
  {
    launched_ = false;
    return {{0, 0}, {0, 0}};//NOLINT
  }
  else
  {
    --launchedCounter_;
    return {beginPoint_, endPoint_};
  }
}

void Chien::Launch(glm::ivec2 beginPoint, glm::ivec2 endPoint)
{
  if(this->cooldownCounter_ == 0)
  {
    launched_ = true;
    launchedCounter_ = LaunchTime;
    beginPoint_ = beginPoint; 
    endPoint_ = endPoint;
    this->cooldownCounter_ = this->cooldown();
  } 
}

bool Chien::launched() const
{
  return launched_;
}
