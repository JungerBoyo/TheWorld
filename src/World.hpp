#ifndef WORLD_HPP
#define WORLD_HPP

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>

#include <array>
#include "Landscape.hpp"
#include "Player.hpp"
#include "Hall.hpp"

struct World : ftxui::ComponentBase
{
  static constexpr std::size_t HallCount{8};

  World(
    std::size_t width, 
    std::size_t height, 
    std::size_t& initialHallIndex,
    std::size_t bottomPanelSize, 
    const std::array<std::string, HallCount>& hallConfigPaths,
    std::function<void()> quitGameCallback,
    std::function<void()> contextSwitchCallback);

  void NextIteration();

  ftxui::Element Render() override;
  bool OnEvent(ftxui::Event) override;
  
private:
  std::size_t width_;
  std::size_t height_;
  std::size_t& currentHallIndex_;

  std::shared_ptr<Landscape> landscape_;

  std::array<Hall, HallCount> halls_;
  std::shared_ptr<Bitmap> currentFrame_;

  std::shared_ptr<Player> player_;
  Box worldBoundaryBox_;

  std::function<void()> quitGameCallback_;
  std::function<void()> contextSwitchCallback_;
};

#endif