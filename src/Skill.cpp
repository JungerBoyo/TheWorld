#include "Skill.hpp"
#include "Entity.hpp"
#include <algorithm>
#include <glm/glm.hpp>


#include <nlohmann/json.hpp>
#include <fstream>


Skill::Skill(Box skillBoundaryBox, std::string_view configFilePath)
  : skillBoundaryBox_(skillBoundaryBox)
{
  std::ifstream stream(configFilePath.data());

  nlohmann::json j;
  stream >> j;
  stream.close();

  dmg_ = j["dmg"].get<int32_t>();
  propagationSpeed_ = j["propagationSpeed"].get<int32_t>();
  length_ = j["length"].get<int32_t>();
  cooldown_ = j["cooldown"].get<int32_t>();
  maxInstances_ = j["maxInstances"].get<std::size_t>();

  instances_.reserve(maxInstances_);

  
  

  for(const auto& jcolor : j["colors"]) 
  {
    glm::u8vec3 result{0,0,0};
    for(std::size_t i{0}; i<3; ++i)
    {
      result[static_cast<int32_t>(i)] = jcolor[i].get<uint8_t>();
    }

    colors_.push_back(result);
  }
}

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

DirectionalRay::DirectionalRay(Box skillBoundaryBox, std::string_view configFilePath)
  : Skill(skillBoundaryBox, configFilePath)
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

    const auto dx = std::abs(instance.p1[0] - instance.p0[0]);
    const auto dy = std::abs(instance.p1[1] - instance.p0[1]);
    const auto sx = instance.p1[0] > instance.p0[0] ? 1 : -1;
    const auto sy = instance.p1[1] > instance.p0[1] ? 1 : -1;
    const auto len = std::min(this->skillLength(), std::max(dx, dy));

    auto error = dx - dy;
    int32_t x{instance.p0[0]};
    int32_t y{instance.p0[1]};
    for (int32_t i{0}; i < len; ++i) 
    {
      if (error >= -dy) 
      {
        error -= dy;
        x += sx;
      }
      if (error <= dx) 
      {
        error += dx;
        y += sy;
      }
    }

    instance.p1 = {x, y};
   
    this->ResetCooldownCounter();
    this->AddInstance(instance);
  }
}