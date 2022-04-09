#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "AnimatedMovePattern.hpp"

struct Player : public Entity
{
  Player(
    glm::ivec2 pos,
    std::string_view standAnimMvJsonFilePath,
    std::string_view walkLeftAnimMvJsonFilePath,
    std::string_view walkRightAnimMvJsonFilePath
  );

  void MoveLeft();
  void MoveRight();
  
  std::shared_ptr<const Bitmap> CurrentSprite() override;

private:
  AnimatedMovePattern standAnimMv_;
  AnimatedMovePattern walkLeftAnimMv_;
  AnimatedMovePattern walkRightAnimMv_;
  //AnimatedMovePattern jumpAnimMv_;

  enum class MoveType { stand, walk_left, walk_right };
  MoveType lastMoveType_;
  
};

#endif