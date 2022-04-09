#include "CollisionValidator.hpp"
#include <tuple>

CollisionValidator::CollisionValidator(glm::ivec2 boxPoint00, glm::ivec2 boxPoint11) 
  : p00_(boxPoint00), p11_(boxPoint11)
{}

static auto FeedbackClamp(int32_t p0, int32_t p1, int32_t lowerConstraint, int32_t upperConstraint)
{
  bool inBounds{ false };

  if(p0 < lowerConstraint)
  {
    auto offset = lowerConstraint - p0;
    p0 += offset;
    p1 += offset;
  }
  else if(p1 > upperConstraint)
  {
    auto offset = p1 - upperConstraint;
    p0 -= offset;
    p1 -= offset;
  }
  else
  {
    inBounds = true;
  }

  return std::make_tuple(p0, p1, inBounds);
}

/*         (boxPoint00)
            |
    (p00_)  |
      +-----↓--+
      |     +--|-+
      |     |  | |
      |     +--|-+ ←--(boxPoint11)
      +--------+
              (p11_)
*/
glm::bvec2 CollisionValidator::TestBox(glm::ivec2& boxPoint00, glm::ivec2 boxPoint11) const
{
  const auto[x0, x1, xInBounds] = FeedbackClamp(boxPoint00[0], boxPoint11[0], p00_[0], p11_[0]);
  const auto[y0, y1, yInBounds] = FeedbackClamp(boxPoint00[1], boxPoint11[1], p00_[1], p11_[1]);

  boxPoint00 = {x0, y0};

  return {xInBounds, yInBounds};
}


