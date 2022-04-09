#ifndef COLLISIONVALIDATOR_HPP
#define COLLISIONVALIDATOR_HPP

#include <glm/vec2.hpp>

struct CollisionValidator
{
  /// boxPoint00 = upper left corner, boxPoint1 = lower right corner, 
  CollisionValidator(glm::ivec2 boxPoint00, glm::ivec2 boxPoint11);

  /// returns if test was passed in x and y axes
  glm::bvec2 TestBox(glm::ivec2& boxPoint00, glm::ivec2 boxPoint11);


private:
  glm::ivec2 p00_;
  glm::ivec2 p11_;
};

#endif