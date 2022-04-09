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

  const std::array<std::string, 8> hallPaths
  {{  
    "game_maps/0.png",
    "game_maps/0.png",
    "game_maps/0.png",
    "game_maps/0.png",
    "game_maps/0.png",
    "game_maps/0.png",
    "game_maps/0.png",
    "game_maps/0.png"
  }};

  auto world = std::make_shared<World>(static_cast<std::size_t>(scr.width() - 2), static_cast<std::size_t>(scr.height() - 2), startupOut, hallPaths, scr.ExitClosure());

  auto gameTabs = ftxui::Container::Tab({startup, world}, &selector);

  scr.TurnOnRefresher();
    scr.Draw(gameTabs);
  scr.TurnOffRefresher();

  return 0;
}