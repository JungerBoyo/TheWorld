#ifndef SKILL_HPP
#define SKILL_HPP

#include <tuple>
#include <memory>
#include <functional>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


#include "Box.hpp"

struct Entity;

struct Skill
{
  struct SkillInstance
  {
    void Move();
    void Cut();

    glm::ivec2 p0;
    glm::ivec2 p1;

    int32_t propagationCounter{0};
  };

  Skill(
    Box skillBoundaryBox,
    int32_t dmg,
    int32_t propagationSpeed, 
    int32_t length, 
    int32_t cooldown, 
    std::size_t maxInstances,
    const std::vector<glm::u8vec3>& colors
  ) 
  : 
    skillBoundaryBox_(skillBoundaryBox),
    dmg_(dmg),
    propagationSpeed_(propagationSpeed),
    length_(length),
    cooldown_(cooldown),
    maxInstances_(maxInstances),
    colors_(colors)
  {
    instances_.reserve(maxInstances);
  }

  virtual void Launch(glm::ivec2 beginPoint, glm::ivec2 endPoint) = 0;
  void Iteration(const std::shared_ptr<Entity>& objective);
  void ForEachInstance(const std::function<void(const SkillInstance&, const std::vector<glm::u8vec3>& colors)>& fn) const;
  
  [[nodiscard]] auto cooldownStateNormalized() const { return static_cast<float>(cooldownCounter_)/static_cast<float>(cooldown_); }
  [[nodiscard]] const auto& colors() const { return colors_; }
 
  virtual ~Skill() = default;

protected: 
  void AddInstance(SkillInstance instance);
  void ResetCooldownCounter();
  [[nodiscard]] auto full() const { return instances_.size() == maxInstances_; }  
  [[nodiscard]] auto skillLength() const { return length_; }
  [[nodiscard]] auto cooldownCounter() const { return cooldownCounter_; }

  Box skillBoundaryBox_;

private:
  int32_t dmg_;
  int32_t propagationSpeed_;
  int32_t length_;
  int32_t cooldown_;
  std::size_t maxInstances_;
  std::vector<SkillInstance> instances_;
  std::vector<glm::u8vec3> colors_;
  
  int32_t cooldownCounter_{0};
};


/*
_____
_____
_____
*/
struct DirectionalRay : public Skill
{
  explicit DirectionalRay(Box skillBoundaryBox);
  void Launch(glm::ivec2 beginPoint, glm::ivec2 endPoint) override;
};


#endif