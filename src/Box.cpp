#include "Box.hpp"
#include <algorithm>
#include <glm/glm.hpp>


static auto onSegment(glm::ivec2 p, glm::ivec2 q, glm::ivec2 r)
{
  return (q[0] <= std::max(p[0], r[0]) && q[0] >= std::min(p[0], r[0]) &&
          q[1] <= std::max(p[1], r[1]) && q[1] >= std::min(p[1], r[1]));
}
 
static auto orientation(glm::ivec2 p, glm::ivec2 q, glm::ivec2 r)
{
  const auto val = (q[1] - p[1]) * (r[0] - q[0]) - (q[0] - p[0]) * (r[1] - q[1]);
  if(val == 0) 
  {
    return 0;  
  }

  return (val > 0) ? 1 : 2; //NOLINT
}

static bool doIntersect(glm::ivec2 p1, glm::ivec2 q1, glm::ivec2 p2, glm::ivec2 q2)
{
    const auto o1 = orientation(p1, q1, p2);
    const auto o2 = orientation(p1, q1, q2);
    const auto o3 = orientation(p2, q2, p1);
    const auto o4 = orientation(p2, q2, q1);

    return (o1 != o2 && o3 != o4)             ||
           (o1 == 0 && onSegment(p1, p2, q1)) ||
           (o3 == 0 && onSegment(p2, p1, q2)) ||
           (o2 == 0 && onSegment(p1, q2, q1)) ||
           (o4 == 0 && onSegment(p2, q1, q2));
}

bool Box::Intersects(Line& line) const  
{ 
  std::array<std::pair<glm::ivec2, glm::ivec2>, 4> boxLines;
  boxLines[0] = {p00_, {p11_[0], p00_[1]}};
  boxLines[1] = {boxLines[0].second, p11_};
  boxLines[2] = {p11_, {p00_[0], p11_[1]}};
  boxLines[3] = {boxLines[2].second, p00_};

  if(doIntersect(boxLines[0].first, boxLines[0].second, line.p0, line.p1))
  {
    line.p1[1] = boxLines[0].first[1] + 2; // NOLINT threshold
    return true;
  }

  if(doIntersect(boxLines[1].first, boxLines[1].second, line.p0, line.p1))
  {
    line.p1[0] = boxLines[0].first[0] - 2; // NOLINT threshold
    return true;
  }

  if(doIntersect(boxLines[2].first, boxLines[2].second, line.p0, line.p1))
  {
    line.p1[1] = boxLines[0].first[1] - 2; // NOLINT threshold
    return true;
  }

  if(doIntersect(boxLines[3].first, boxLines[3].second, line.p0, line.p1))
  {
    line.p1[0] = boxLines[0].first[0] + 2; // NOLINT threshold
    return true;
  }

  return false;
}

bool Box::IsIn(glm::ivec2 p) const 
{
  return  p00_[0] < p[0] && 
          p11_[0] > p[0] && 
          p00_[1] < p[1] && 
          p11_[1] > p[1];
}

void Box::ShiftToFitIn(const Box& other) 
{
  if(xMax() - xMin() < other.xMax() - other.xMin() &&
     yMax() - yMin() < other.yMax() - other.yMin())
  {
    glm::ivec2 dxy{0, 0};
    if(xMin() < other.xMin())
    {
      dxy[0] = other.xMin() - xMin();
    }
    else if(xMax() > other.xMax())
    {
      dxy[0] = other.xMax() - xMax();
    }

    if(yMin() < other.yMin())
    {
      dxy[1] = other.yMin() - yMin();
    }
    else if(yMax() > other.yMax())
    {
      dxy[1] = other.yMax() - yMax();
    }

    p00_ += dxy;
    p11_ += dxy;
  }
}
