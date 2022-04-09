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

glm::bvec2 CollisionValidator::TestBox(glm::ivec2& boxPoint00, glm::ivec2 boxPoint11) 
{
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

  const auto[x0, x1, xInBounds] = FeedbackClamp(boxPoint00.x, boxPoint11.x, p00_.x, p11_.x);
  const auto[y0, y1, yInBounds] = FeedbackClamp(boxPoint00.y, boxPoint11.y, p00_.y, p11_.y);

  boxPoint00 = {x0, y0};

  return {xInBounds, yInBounds};
}


