#ifndef SKILL_HPP
#define SKILL_HPP

#include <tuple>
#include <glm/vec2.hpp>

struct Skill
{
  enum class Type { lines, points };

  Skill(int32_t propagationSpeed, int32_t len, int32_t width, int32_t cooldown, Type type)
    : propagationSpeed_(propagationSpeed),
      len_(len),
      width_(width),
      cooldown_(cooldown),
      type_(type){}
  
  /// returns begin point and end point
  virtual std::pair<glm::ivec2, glm::ivec2> Iteration() = 0;
  virtual void Launch(glm::ivec2 beginPoint, glm::ivec2 endPoint) = 0;

  [[nodiscard]] auto width() const { return width_; }
  [[nodiscard]] auto type() const { return type_; }
  [[nodiscard]] auto cooldownStateNormalized() const { return static_cast<float>(cooldownCounter_)/static_cast<float>(cooldown_); };

  [[nodiscard]] virtual bool launched() const = 0;

  virtual ~Skill() = default; 

protected:
  [[nodiscard]] auto propagationSpeed() const { return propagationSpeed_; }
  [[nodiscard]] auto length() const { return len_; }
  [[nodiscard]] auto cooldown() const { return cooldown_; }

  int32_t cooldownCounter_{0};

private:
  int32_t propagationSpeed_;
  int32_t len_;
  int32_t width_;
  int32_t cooldown_;
  Type type_;
};

/*
  Directional Ray
  _____
  _____
  _____
*/
struct Chien : public Skill
{
  Chien();

  void Launch(glm::ivec2 beginPoint, glm::ivec2 endPoint) override;
  std::pair<glm::ivec2, glm::ivec2> Iteration() override;

  [[nodiscard]] bool launched() const override;

private:
  glm::ivec2 beginPoint_{0, 0};
  glm::ivec2 endPoint_{0, 0};

  static constexpr int32_t LaunchTime{3};
  int32_t launchedCounter_{0};

  bool launched_{false};
};

#endif