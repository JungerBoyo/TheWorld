#ifndef LINE_HPP
#define LINE_HPP

#include <glm/vec2.hpp>

struct Line
{
  Line(glm::ivec2 P0, glm::ivec2 P1);

  int32_t yAt(int32_t x) const;

  [[nodiscard]] auto xMax() const { return xMax_; }
  [[nodiscard]] auto xMin() const { return xMin_; }
  [[nodiscard]] auto yMax() const { return yMax_; }
  [[nodiscard]] auto yMin() const { return yMin_; }

  glm::ivec2 p0; 
  glm::ivec2 p1;

private:

  int32_t xMax_;
  int32_t xMin_;
  int32_t yMax_;
  int32_t yMin_;  

  float m_;
  float b_;
};

#endif
