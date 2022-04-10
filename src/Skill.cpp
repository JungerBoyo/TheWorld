#include "Skill.hpp"
#include "Entity.hpp"

#include <glm/glm.hpp>

void Skill::SkillInstance::Move()
{
  const auto dx = std::abs(p1[0] - p0[0]);
  const auto dy = std::abs(p1[1] - p0[1]);
  const auto sx = p1[0] > p0[0] ? 1 : -1;
  const auto sy = p1[1] > p0[1] ? 1 : -1;
  
  auto error = dx - dy;

  if (error >= -dy) 
  {
    error -= dy;
    p0[0] += sx;
    p1[0] += sx;
  }

  if (error <= dx) 
  {
    p0[1] += sy;
    p1[1] += sy;
  }
}

void Skill::SkillInstance::Cut()
{
  const auto dx = std::abs(p1[0] - p0[0]);
  const auto dy = std::abs(p1[1] - p0[1]);
  const auto sx = p1[0] > p0[0] ? 1 : -1;
  const auto sy = p1[1] > p0[1] ? 1 : -1;
  
  auto error = dx - dy;

  if (error >= -dy) 
  {
    error -= dy;
    p1[0] -= sx;
  }

  if (error <= dx) 
  {
    p1[1] -= sy;
  }
}


void Skill::Iteration(const std::shared_ptr<Entity>& objective) 
{
  if(cooldownCounter_ > 0)
  {
    --cooldownCounter_;
  }
  
  if(objective)
  {
    const auto objectiveBox = objective->boundingBox(); 

    for(std::size_t i{0}; i<instances_.size(); ++i)
    {
      auto& instance = instances_.at(i);

      auto line = Line(instance.p0, instance.p1);

      const auto reachedBoundary = skillBoundaryBox_.Intersects(line);
      const auto reachedObjective = objectiveBox.Intersects(line);
      if(reachedObjective)
      {
        objective->InflictDmg(dmg_);
      }

      if(reachedObjective || reachedBoundary)
      {  
        instances_.erase(std::next(instances_.begin(), static_cast<signed>(i)));
      }
      else
      {
        if(instance.propagationCounter == 0)
        {
          instance.propagationCounter = propagationSpeed_;

          instance.Move();
        }
        else
        {
          --instance.propagationCounter; 
        }
      }
    }
  }
  else
  {
    for(std::size_t i{0}; i<instances_.size(); ++i)
    {
      auto& instance = instances_.at(i);

      auto line = Line(instance.p0, instance.p1);
      const auto reachedBoundary = skillBoundaryBox_.Intersects(line);

      if(reachedBoundary)
      {
        instances_.erase(std::next(instances_.begin(), static_cast<signed>(i)));
      }
      else
      {
        if(instance.propagationCounter == 0)
        {
          instance.propagationCounter = propagationSpeed_;

          instance.Move();
        }
        else
        {
          --instance.propagationCounter; 
        }
      }
    }
  }
 
}

void Skill::ForEachInstance(const std::function<void(const SkillInstance&, const std::vector<glm::u8vec3>& colors)>& fn) const
{ 
  for(const auto& instance : instances_)
  {
    fn(instance, this->colors());
  }
}

void Skill::ResetCooldownCounter()
{
  cooldownCounter_ = cooldown_;
}

void Skill::AddInstance(SkillInstance instance)
{
  instances_.push_back(instance);
}

/// DIRECTIONAL RAY ///

DirectionalRay::DirectionalRay(Box skillBoundaryBox)
  : Skill(skillBoundaryBox, 10, 5, 50, 150, 1, {{255, 0, 0}, {255, 125, 0}})// NOLINT
{}

void DirectionalRay::Launch(glm::ivec2 beginPoint, glm::ivec2 endPoint) 
{
  if(this->cooldownCounter() == 0 && !this->full())
  {
    SkillInstance instance;
    instance.propagationCounter = 0;
    instance.p0 = beginPoint;
    instance.p1 = endPoint;

    if(!skillBoundaryBox_.IsIn(instance.p0) || !skillBoundaryBox_.IsIn(instance.p1))
    {
      return;
    }

    //instance.p1 = static_cast<glm::ivec2>(instance.dir * static_cast<float>(this->skillLength())); 
//
    //if(!skillBoundaryBox_.IsIn(instance.p1))
    //{
    //  instance.p1[0] = std::clamp(instance.p1[0], skillBoundaryBox_.xMin() + 1, skillBoundaryBox_.xMax() - 1);
    //  instance.p1[1] = std::clamp(instance.p1[1], skillBoundaryBox_.yMin() + 1, skillBoundaryBox_.yMax() - 1);
    //}
   
    this->ResetCooldownCounter();
    this->AddInstance(instance);
  }
}