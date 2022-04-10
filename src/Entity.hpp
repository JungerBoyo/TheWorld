#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <memory>
#include <cinttypes>
#include <glm/vec2.hpp>

#include "Bitmap.hpp"
#include "Skill.hpp"

struct Entity
{
  Entity(glm::ivec2 pos, glm::ivec2 extent, int32_t health);
  
  virtual bool InflictDmg(int32_t dmg, int32_t cooldown);
  virtual void CommitNextPos([[maybe_unused]] glm::bvec2 xyUnchanged, glm::ivec2 nextPos);

  virtual std::shared_ptr<const Bitmap> CurrentSprite() = 0;

  void SetSkill(std::shared_ptr<Skill> skill);
  void LaunchSkill(glm::ivec2 endPoint);

  [[nodiscard]] auto pos() const { return pos_; }
  [[nodiscard]] auto nextPos() const { return pos_ + accumMv_; }
  [[nodiscard]] auto extent() const { return extent_; }
  [[nodiscard]] auto skill() const { return skill_; }
  [[nodiscard]] auto healthStateNormalized() const { return static_cast<float>(health_)/static_cast<float>(fullHealth_); }
 
  virtual ~Entity() = default;

protected:
  void AccumulateNextMove(glm::ivec2 mv);  

private:
  std::shared_ptr<Skill> skill_;

  glm::ivec2 pos_;
  glm::ivec2 extent_;

  int32_t fullHealth_;
  int32_t health_;

  int32_t dmgCooldownCounter_{ 0 };

  glm::ivec2 accumMv_{0, 0};
};

#endif