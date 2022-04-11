#include "Startup.hpp"
#include "World.hpp"
#include "Screen.hpp"
#include <fmt/format.h>

int main()
{
  Screen scr(120, 50, 40);// NOLINT

  std::size_t startupOut{ 0 };
  int selector{0};

  auto startup = std::make_shared<Startup>("game_startup_text_data/data.json", startupOut, [&selector]{ selector = 1; });

  std::array<std::string, World::HallCount> hallConfigPaths;
  hallConfigPaths[0] = "game_hall_config_files/hall_0_config.json";// NOLINT
  hallConfigPaths[1] = "game_hall_config_files/hall_1_config.json";// NOLINT
  hallConfigPaths[2] = "game_hall_config_files/hall_2_config.json";// NOLINT
  hallConfigPaths[3] = "game_hall_config_files/hall_3_config.json";// NOLINT
  hallConfigPaths[4] = "game_hall_config_files/hall_4_config.json";// NOLINT
  hallConfigPaths[5] = "game_hall_config_files/hall_5_config.json";// NOLINT
  hallConfigPaths[6] = "game_hall_config_files/hall_6_config.json";// NOLINT
  hallConfigPaths[7] = "game_hall_config_files/hall_7_config.json";// NOLINT

  constexpr std::size_t gameBottomPanelSize = 13;

  auto world = std::make_shared<World>(
    static_cast<std::size_t>(scr.width() - 2), 
    static_cast<std::size_t>(scr.height() - 2), 
    startupOut, gameBottomPanelSize, 
    hallConfigPaths, 
    scr.ExitClosure(),
    [&selector]{ selector = 2; });// NOLINT

  auto endScrLandscape = std::make_shared<Landscape>(static_cast<std::size_t>(scr.width() - 2), static_cast<std::size_t>(scr.height() - 2));
  endScrLandscape->SetBackground(std::make_shared<Bitmap>("game_maps/end_screen.png"));
  endScrLandscape->SetForeground(std::make_shared<Bitmap>("game_maps/end_screen.png"));
  
  auto endScrRenderer = ftxui::CatchEvent(
    ftxui::Renderer([&endScrLandscape]{ return endScrLandscape; }), 
    [callback = scr.ExitClosure()](const ftxui::Event& ev){
      if(ev.is_character())
      {
        if(tolower(static_cast<int>(ev.character().front())) == 'q')
        {
          callback();
          return true;
        }
      }

      return false;
    });

  auto gameTabs = ftxui::Container::Tab({startup, world, endScrRenderer}, &selector);

  scr.TurnOnRefresher();
    scr.Draw(gameTabs);
  scr.TurnOffRefresher();

  return 0;
}