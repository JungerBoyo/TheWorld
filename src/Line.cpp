#include "Line.hpp"
#include <algorithm>


Line::Line(glm::ivec2 P0, glm::ivec2 P1)  
  :
  p0(P0),
  p1(P1),

  xMax_(std::max(p0[0], p1[0])),
  xMin_(std::min(p0[0], p1[0])),
  yMax_(std::max(p0[1], p1[1])),
  yMin_(std::min(p0[1], p1[1])),

  m_( static_cast<float>(p1[1] - p0[1])/static_cast<float>(p1[0] - p0[0]) ),
  b_( static_cast<float>(p1[1]) - static_cast<float>(p1[0]) * m_)
{}

int32_t Line::yAt(int32_t x) const 
{
  return static_cast<int32_t>(m_ * static_cast<float>(x) + b_);
}
