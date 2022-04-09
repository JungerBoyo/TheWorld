#include "AnimatedMovePattern.hpp"

#include <nlohmann/json.hpp>
#include <fstream>

AnimatedMovePattern::AnimatedMovePattern(std::string_view jsonDataFilePath) 
{
  std::ifstream stream(jsonDataFilePath.data());

  assert(stream.good());

  nlohmann::json j;
  stream >> j;
  stream.close();

  for(const auto& value : j)
  {
    Frame frame;
    for(const auto& jMove : value["moves"])
    { 
      Move move{};

      const auto mv = jMove["move"].get<std::pair<int32_t, int32_t>>();
      move.vec[0] = mv.first;
      move.vec[1] = mv.second;

      move.timeout = jMove["fps_timeout"].get<uint32_t>();
      move.repeats = jMove["repeats"].get<uint32_t>();

      frame.moves.push_back(move);
    }

    frame.sprite = std::make_shared<Bitmap>(value["sprite_path"].get<std::string>());
  
    frames_.push_back(frame);
  }

  currentFrameIndex_ = 0;
  currentMoveIndex_ = 0;

  const auto& firstMove =  frames_.front().moves.front();
  currentRepeatCounter_ = firstMove.repeats;
  currentTimeoutCounter_ = firstMove.timeout;
}

/*
  |                                 frame
  |       sprite          |         .....        |      ..
  |                       |         ...
  | move | move | move |..|   
  | rep  | rep  | rep
  | timo | ...  | ...


  sprites
    moves
      repeats
        timeouts

*/
std::pair<std::shared_ptr<const Bitmap>, glm::ivec2> AnimatedMovePattern::Iteration(bool& ended) 
{
  ended = false;

  const auto& frame = frames_[currentFrameIndex_];
  const auto& move = frame.moves[currentMoveIndex_];

  auto result = (currentTimeoutCounter_ == move.timeout) ? std::pair{frame.sprite, move.vec} : std::pair{frame.sprite, glm::ivec2{0, 0}};

  if(currentTimeoutCounter_ == 0)
  {
    if(currentRepeatCounter_ == 0)
    {
      if(currentMoveIndex_ + 1 == frame.moves.size())
      {
        if(currentFrameIndex_ + 1 == frames_.size())
        {
          ended = true;
          currentFrameIndex_ = 0;
          currentMoveIndex_ = 0;

        }
        else
        {
          ++currentFrameIndex_;
          currentMoveIndex_ = 0;
        }
      }
      else
      {
        ++currentMoveIndex_;
      }

      const auto& nextMove = frames_[currentFrameIndex_].moves[currentMoveIndex_];
      currentTimeoutCounter_ = nextMove.timeout;
      currentRepeatCounter_ = nextMove.repeats;
    }
    else
    {
      currentTimeoutCounter_ = move.timeout;
      --currentRepeatCounter_;
    }
  }
  else
  {
    --currentTimeoutCounter_; 
  }

  return result;
}

std::pair<std::shared_ptr<const Bitmap>, glm::ivec2> AnimatedMovePattern::Iteration() 
{
  bool dummy{false};
  return Iteration(dummy);    
}
