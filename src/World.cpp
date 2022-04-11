#include "World.hpp"

#include <fmt/format.h>
#include <algorithm>

World::World(
  std::size_t width, 
  std::size_t height, 
  std::size_t& initialHallIndex,
  std::size_t bottomPanelSize, 
  const std::array<std::string, HallCount>& hallConfigPaths,
  std::function<void()> quitGameCallback)
: 
    width_(width),
    height_(height),
    
    currentHallIndex_(initialHallIndex),
    
    landscape_(std::make_shared<Landscape>(width_, height_ - bottomPanelSize)),
    
    player_(std::make_shared<Player>(
               glm::ivec2{static_cast<int32_t>(width/2), static_cast<int32_t>(height - 13 - bottomPanelSize)},// NOLINT ground level = 9px
              "game_animations/Player/stand/data.json",
              "game_animations/Player/move_left/data.json",
              "game_animations/Player/move_right/data.json",
              "game_animations/Player/jump/data.json"
            )),
  
    worldBoundaryBox_({0, 0}, {width, height - 9 - bottomPanelSize}), // NOLINT width of the ground = 9px
    quitGameCallback_(std::move(quitGameCallback))
{
  for(std::size_t i{0}; i<hallConfigPaths.size(); ++i)
  {
    halls_.at(i) = Hall(
      hallConfigPaths.at(i), 
      glm::ivec2{static_cast<int32_t>(width)/2, static_cast<int32_t>(height_ - bottomPanelSize)/2},
      worldBoundaryBox_.p00(), 
      worldBoundaryBox_.p11(),
      player_);
  }

  currentFrame_ = std::make_shared<Bitmap>(width, height - bottomPanelSize);

  landscape_->SetForeground(currentFrame_);
  landscape_->SetBackground(halls_.at(currentHallIndex_).map);

  auto* skillCast = dynamic_cast<DirectionalRay*>(halls_.at(currentHallIndex_).boss->skill().get());
  if(skillCast != nullptr)
  {
    player_->SetSkill(std::make_shared<DirectionalRay>(*skillCast));
  }
}

void World::NextIteration()
{
  /// DRAWING ///
    /// PLAYER ///
  const auto playerSprite = player_->CurrentSprite();
  const auto playerPos = player_->pos();
  const auto playerSkill = player_->skill();

  currentFrame_->DrawOther(static_cast<std::size_t>(playerPos[0]), static_cast<std::size_t>(playerPos[1]), *playerSprite);
  
  const auto drawFn{
  [frame = currentFrame_](const Skill::SkillInstance& value, const std::vector<glm::u8vec3>& colors){
    frame->DrawLine(value.p0, value.p1, colors);
  }};

  playerSkill->ForEachInstance(drawFn);

    /// BOSS ///
  if(halls_.at(currentHallIndex_).active)
  {
    auto& hall = halls_.at(currentHallIndex_);

    const auto bossSprite = hall.boss->CurrentSprite();
    const auto bossPos = hall.boss->pos();
 
    currentFrame_->DrawOther(static_cast<std::size_t>(bossPos[0]), static_cast<std::size_t>(bossPos[1]), *bossSprite);
    hall.boss->skill()->ForEachInstance(drawFn);
  }
  

  /// ITERATION ///
    /// PLAYER /// 
  auto playerNextPos00 = player_->nextPos();
  auto playerNextPos11 = playerNextPos00 + player_->extent();

  if(playerNextPos00[0] == worldBoundaryBox_.xMin())// NOLINT
  {
    playerNextPos00[0] = worldBoundaryBox_.xMax() - player_->extent().x - 1;// NOLINT
    playerNextPos11[0] = playerNextPos00[0] + player_->extent()[0];
   
    auto& hall = halls_.at(currentHallIndex_);
    hall.active = false;
    hall.boss->RestoreHealth();

    currentHallIndex_ = currentHallIndex_ == 0 ? HallCount - 1 : currentHallIndex_ - 1;// NOLINT
    landscape_->SetBackground(halls_.at(currentHallIndex_).map);
  }
  else if(playerNextPos11[0] == worldBoundaryBox_.xMax())// NOLINT
  {
    playerNextPos00[0] = worldBoundaryBox_.xMin() + 2;// NOLINT
    playerNextPos11[0] = playerNextPos00[0] + player_->extent()[0];

    auto& hall = halls_.at(currentHallIndex_);
    hall.active = false;
    hall.boss->RestoreHealth();

    currentHallIndex_ = (currentHallIndex_ + 1) % HallCount;// NOLINT
    landscape_->SetBackground(halls_.at(currentHallIndex_).map);
  }

  Box pBox(playerNextPos00, playerNextPos11);

  pBox.ShiftToFitIn(worldBoundaryBox_);

  player_->SetPosition(pBox.p00());

  if(!halls_.at(currentHallIndex_).active)
  {
    playerSkill->Iteration(nullptr);
  }
  else 
  /// BOSS ///
  { 
    auto& hall = halls_.at(currentHallIndex_);

    playerSkill->Iteration(hall.boss);

    const auto bossNextPos00 = hall.boss->nextPos();
    const auto bossNextPos11 = bossNextPos00 + hall.boss->extent();

    Box bossBox(bossNextPos00, bossNextPos11);

    bossBox.ShiftToFitIn(worldBoundaryBox_);

    hall.boss->SetPosition(bossBox.p00());

    hall.boss->skill()->Iteration(player_);
  } 
}

ftxui::Element World::Render()
{
  currentFrame_->Clear();
  NextIteration();
  auto& hall = halls_.at(currentHallIndex_);

  return ftxui::vbox({
    landscape_ | ftxui::border,
    ftxui::hbox({
      ftxui::filler(),
      ftxui::vbox({
        ftxui::text("your health points") | ftxui::center | ftxui::bold,
        ftxui::gauge(player_->healthStateNormalized()) | ftxui::border
      }),
      ftxui::filler(),
      ftxui::separator(),
      ftxui::filler(),
      ftxui::vbox({
        ftxui::text("your skill cooldown") | ftxui::center | ftxui::bold,
        ftxui::gauge(player_->skill()->cooldownStateNormalized())  | ftxui::border
        
      }),
      ftxui::filler(),
      ftxui::separator(),
      ftxui::vbox({
        ftxui::hbox({
          ftxui::text("<\'a\'")   | ftxui::border,
          ftxui::text("jump \'w\'") | ftxui::border,
          ftxui::text("\'d\'> ")  | ftxui::border,
          ftxui::text("quit \'p\'")  | ftxui::border
        }) | ftxui::center,
        ftxui::hbox({
          ftxui::text("skill lhs button") | ftxui::border,
          ftxui::text("invoke boss \'i\'") | ftxui::border
        })
      }) | ftxui::center ,
      ftxui::separator(), 
      ftxui::vbox({
        ftxui::text("     " + hall.boss->name() + "     ") | ftxui::border | ftxui::bold | ftxui::center,
        ftxui::gauge(hall.active ? hall.boss->healthStateNormalized() : 0.f)  | ftxui::border // NOLINT
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
      case 'p': 
          quitGameCallback_();
        return true;

      case 'w': 
          player_->Jump();
        return true;

      case 'a': 
          player_->MoveLeft();
        return true;

      case 'd': 
          player_->MoveRight();
        return true;
      
      case 'i':
          halls_.at(currentHallIndex_).active = true;
        return true;

      default: break;
    }
  }
  else if(ev.is_mouse() && ev.mouse().button == ftxui::Mouse::Left)
  {
    const auto x = std::clamp(ev.mouse().x - 1, 0, static_cast<int32_t>(width_ - 1));// NOLINT
    const auto y = std::clamp(ev.mouse().y - 1, 0, static_cast<int32_t>(height_ - 1));// NOLINT

    player_->LaunchSkill({x, y});

    return true;
  }

  return false;
}