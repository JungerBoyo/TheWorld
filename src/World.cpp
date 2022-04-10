#include "World.hpp"

#include <fmt/format.h>

World::World(
  std::size_t width, 
  std::size_t height, 
  std::size_t& initialHallIndex,
  std::size_t bottomPanelSize, 
  const std::array<std::string, HallCount>& hallPaths,
  std::function<void()> quitGameCallback)
: 
    width_(width),
    height_(height),
    
    currentHallIndex_(initialHallIndex),
    
    landscape_(std::make_shared<Landscape>(width_, height_ - bottomPanelSize)),
    
    player_({static_cast<int32_t>(width/2), static_cast<int32_t>(height - 13 - bottomPanelSize)},// NOLINT ground level = 9px
              "game_animations/Player/stand/data.json",
              "game_animations/Player/move_left/data.json",
              "game_animations/Player/move_right/data.json"
            ),
    
    collisionValidator_({0, 0}, {width, height - 9 - bottomPanelSize}), // NOLINT width of the ground = 9px
    quitGameCallback_(std::move(quitGameCallback))
{
  for(std::size_t i{0}; i<hallPaths.size(); ++i)
  {
    halls_.at(i) = std::make_shared<Bitmap>(hallPaths.at(i));
  }

  currentFrame_ = std::make_shared<Bitmap>(width, height - bottomPanelSize);

  landscape_->SetForeground(currentFrame_);
  landscape_->SetBackground(halls_.at(currentHallIndex_));


  player_.SetSkill(std::make_shared<Chien>());
}

void World::NextIteration()
{
  const auto sprite = player_.CurrentSprite();
  const auto pos = player_.pos();

  //assert(pos.x >= 0 && pos.y >= 0);// DEBUG

  currentFrame_->DrawOther(static_cast<std::size_t>(pos[0]), static_cast<std::size_t>(pos[1]), *sprite);

  auto[beginPoint, endPoint] = player_.skill()->Iteration();
  collisionValidator_.TestLine(beginPoint, endPoint);

  Pixel px{};
  px.color = {0, 0, 0};// NOLINT
  px.ch = U'█';

  currentFrame_->DrawLine(beginPoint, endPoint, px);
  

  auto nextPos00 = player_.nextPos();
  auto nextPos11 = nextPos00 + player_.extent();

  const auto testResult = collisionValidator_.TestBox(nextPos00, nextPos11);
  player_.CommitNextPos(testResult, nextPos00);
}
/*
  p1 


        p2
*/

ftxui::Element World::Render()
{
  currentFrame_->Clear();
  NextIteration();

  return ftxui::vbox({
    landscape_ | ftxui::border,
    ftxui::hbox({
      ftxui::filler(),
      ftxui::vbox({
        ftxui::text("your health points") | ftxui::center | ftxui::bold,
        ftxui::gauge(player_.healthStateNormalized()) | ftxui::border
      }),
      ftxui::filler(),
      ftxui::separator(),
      ftxui::filler(),
      ftxui::vbox({
        ftxui::text("your skill cooldown") | ftxui::center | ftxui::bold,
        ftxui::gauge(player_.skill()->cooldownStateNormalized())  | ftxui::border
        
      }),
      ftxui::filler(),
      ftxui::separator(),
      ftxui::vbox({
        ftxui::hbox({
          ftxui::text("<< \'a\'")   | ftxui::border,
          ftxui::text("jump \'w\'") | ftxui::border,
          ftxui::text("\'d\' >> ")  | ftxui::border
        }),
        ftxui::text("skill => lhs mouse button") | ftxui::border
      }) | ftxui::center ,
      ftxui::separator(), 
      ftxui::vbox({
        ftxui::text("here goes boss name") | ftxui::border | ftxui::bold | ftxui::center,
        ftxui::gauge(0.f)  | ftxui::border // NOLINT
      }) | ftxui::center
    }) | ftxui::border});
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
  else if(ev.is_mouse() && ev.mouse().button == ftxui::Mouse::Left)
  {
    const auto x = std::clamp(ev.mouse().x - 1, 0, static_cast<int32_t>(width_ - 1));// NOLINT
    const auto y = std::clamp(ev.mouse().y - 1, 0, static_cast<int32_t>(height_ - 1));// NOLINT

    player_.LaunchSkill({x, y});
  }

  return false;
}