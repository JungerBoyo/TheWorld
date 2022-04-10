#ifndef BOX_HPP
#define BOX_HPP

#include "Line.hpp"

struct Box
{
  Box(glm::ivec2 p00, glm::ivec2 p11)
    : p00_(p00), p11_(p11) {}

  bool Intersects(Line& line) const;
  void ShiftToFitIn(const Box& other);
  bool IsIn(glm::ivec2 p) const;
  
  [[nodiscard]] auto p00() const { return p00_; }
  [[nodiscard]] auto p11() const { return p11_; }

  [[nodiscard]] auto xMax() const { return p11_[0]; }
  [[nodiscard]] auto xMin() const { return p00_[0]; }
  [[nodiscard]] auto yMax() const { return p11_[1]; }
  [[nodiscard]] auto yMin() const { return p00_[1]; }

private:
  glm::ivec2 p00_;
  glm::ivec2 p11_;
};

#endif