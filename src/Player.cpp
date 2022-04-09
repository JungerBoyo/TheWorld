#include "Player.hpp"

Player::Player(
  glm::ivec2 pos,
  std::string_view standAnimMvJsonFilePath,
  std::string_view walkLeftAnimMvJsonFilePath,
  std::string_view walkRightAnimMvJsonFilePath
)
  : Entity(pos, {3, 4}, 100),//NOLINT

    standAnimMv_(standAnimMvJsonFilePath),
    walkLeftAnimMv_(walkLeftAnimMvJsonFilePath),
    walkRightAnimMv_(walkRightAnimMvJsonFilePath)
{
  lastMoveType_ = MoveType::stand;
}
  
void Player::MoveLeft()
{
  lastMoveType_ = MoveType::walk_left;
}

void Player::MoveRight()
{
  lastMoveType_ = MoveType::walk_right;
}

std::shared_ptr<const Bitmap> Player::CurrentSprite()
{
  std::shared_ptr<const Bitmap> result;
  glm::ivec2 move;

  switch (lastMoveType_)
  {
    case MoveType::stand:      std::tie(result, move) = standAnimMv_.Iteration();     break;
    case MoveType::walk_left:  std::tie(result, move) = walkLeftAnimMv_.Iteration();  break;
    case MoveType::walk_right: std::tie(result, move) = walkRightAnimMv_.Iteration(); break;

    default:
      break;
  }

  this->AccumulateNextMove(move);

  return result;
}