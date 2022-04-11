#include "Player.hpp"

Player::Player(
  glm::ivec2 pos,
  std::string_view standAnimMvJsonFilePath,
  std::string_view walkLeftAnimMvJsonFilePath,
  std::string_view walkRightAnimMvJsonFilePath,
  std::string_view jumpAnimMvJsonFilePath
)
  : Entity(pos, {3, 4}, 3, 100),//NOLINT

    standAnimMv_(standAnimMvJsonFilePath),
    walkLeftAnimMv_(walkLeftAnimMvJsonFilePath),
    walkRightAnimMv_(walkRightAnimMvJsonFilePath),
    jumpAnimMv_(jumpAnimMvJsonFilePath)
{
  lastMoveType_ = MoveType::stand;
}
  
void Player::MoveLeft()
{
  if(lastMoveType_ != MoveType::jump)
  {
    lastMoveType_ = MoveType::walk_left;
  } 
}

void Player::MoveRight()
{
  if(lastMoveType_ != MoveType::jump)
  {
    lastMoveType_ = MoveType::walk_right;
  }
}

void Player::Jump()
{
  lastMoveType_ = MoveType::jump;
}

std::shared_ptr<const Bitmap> Player::CurrentSprite()
{
  std::shared_ptr<const Bitmap> result;
  glm::ivec2 move;

  if(lastMoveType_ == MoveType::stand)
  {
    std::tie(result, move) = standAnimMv_.Iteration();     
  }
  else if(lastMoveType_ == MoveType::walk_left)
  {
    bool ended{false};
    std::tie(result, move) = walkLeftAnimMv_.Iteration(ended);  

    if(ended)
    {
      lastMoveType_ = MoveType::stand;
    }
  }
  else if(lastMoveType_ == MoveType::walk_right)
  {
    bool ended{false};
    std::tie(result, move) = walkRightAnimMv_.Iteration(ended);  

    if(ended)
    {
      lastMoveType_ = MoveType::stand;
    }
  }
  else if(lastMoveType_ == MoveType::jump)
  {
    bool ended{false};
    std::tie(result, move) = jumpAnimMv_.Iteration(ended);  

    if(ended)
    {
      lastMoveType_ = MoveType::stand;
    }
  }

  this->AccumulateNextMove(move);

  return result;
}