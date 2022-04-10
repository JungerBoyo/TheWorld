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

  std::array<std::string, World::HallCount> hallPaths;
  hallPaths[0] = "game_maps/0.png";// NOLINT
  hallPaths[1] = "game_maps/0.png";// NOLINT
  hallPaths[2] = "game_maps/0.png";// NOLINT
  hallPaths[3] = "game_maps/0.png";// NOLINT
  hallPaths[4] = "game_maps/0.png";// NOLINT
  hallPaths[5] = "game_maps/0.png";// NOLINT
  hallPaths[6] = "game_maps/0.png";// NOLINT
  hallPaths[7] = "game_maps/0.png";// NOLINT

  constexpr std::size_t gameBottomPanelSize = 13;

  auto world = std::make_shared<World>(static_cast<std::size_t>(scr.width() - 2), static_cast<std::size_t>(scr.height() - 2), startupOut, gameBottomPanelSize, hallPaths, scr.ExitClosure());

  auto gameTabs = ftxui::Container::Tab({startup, world}, &selector);

  scr.TurnOnRefresher();
    scr.Draw(gameTabs);
  scr.TurnOffRefresher();

  return 0;
}