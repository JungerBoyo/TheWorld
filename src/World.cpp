#include "World.hpp"

World::World(
  std::size_t width, 
  std::size_t height, 
  std::size_t& initialHallIndex,
  const std::array<std::string, HallCount>& hallPaths,
  std::function<void()> quitGameCallback)
: 
    width_(width),
    height_(height),
    
    currentHallIndex_(initialHallIndex),
    
    landscape_(std::make_shared<Landscape>(width_, height_)),
    
    player_({static_cast<int32_t>(width/2), static_cast<int32_t>(height - 13)},// NOLINT ground level = 10px
              "game_animations/Player/stand/data.json",
              "game_animations/Player/move_left/data.json",
              "game_animations/Player/move_right/data.json"
            ),
    
    collisionValidator_({0, 0}, {width, height}),
    quitGameCallback_(std::move(quitGameCallback))
{
  for(std::size_t i{0}; i<hallPaths.size(); ++i)
  {
    halls_.at(i) = std::make_shared<Bitmap>(hallPaths.at(i));
  }

  currentFrame_ = std::make_shared<Bitmap>(width, height);

  landscape_->SetForeground(currentFrame_);
  landscape_->SetBackground(halls_.at(currentHallIndex_));
}

void World::NextIteration()
{
  const auto sprite = player_.CurrentSprite();
  const auto pos = player_.pos();

  assert(pos.x >= 0 && pos.y >= 0);

  currentFrame_->DrawOther(static_cast<std::size_t>(pos[0]), static_cast<std::size_t>(pos[1]), *sprite);

  auto nextPos = player_.nextPos();

  const auto testResult = collisionValidator_.TestBox(nextPos, nextPos + player_.extent());
  player_.CommitNextPos(testResult, nextPos);
}

ftxui::Element World::Render()
{
  currentFrame_->Clear();
  NextIteration();

  return landscape_ | ftxui::border;
}

bool World::OnEvent(ftxui::Event ev)
{
  if(ev.is_character())
  {
    auto ch = tolower(static_cast<int>(ev.character().front()));

    switch(ch)
    {
      case 'q': 
          quitGameCallback_();
        return true;

      case 'w': 

        return true;

      case 'a': 
          player_.MoveLeft();
        return true;

      case 'd': 
          player_.MoveRight();
        return true;
      
      default: break;
    }
  }

  return false;
}