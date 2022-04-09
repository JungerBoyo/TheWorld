#ifndef ANIMATEDMOVEPATTERN_HPP
#define ANIMATEDMOVEPATTERN_HPP

#include <string_view>
#include <memory>
#include <glm/vec2.hpp>

#include "Bitmap.hpp"

struct AnimatedMovePattern
{
  explicit AnimatedMovePattern(std::string_view jsonDataFilePath);

  /// returns iteration sprite and iteration move
  std::pair<std::shared_ptr<const Bitmap>, glm::ivec2> Iteration(bool& ended);
  std::pair<std::shared_ptr<const Bitmap>, glm::ivec2> Iteration();

private:
  struct Move
  {
    uint32_t timeout;
    uint32_t repeats;
    glm::ivec2 vec;
  };

  struct Frame
  {
    std::shared_ptr<const Bitmap> sprite;
    std::vector<AnimatedMovePattern::Move> moves;  
  };  

  std::vector<Frame> frames_;

  uint32_t currentTimeoutCounter_;
  uint32_t currentRepeatCounter_;
  std::size_t currentFrameIndex_;
  std::size_t currentMoveIndex_;
};

#endif