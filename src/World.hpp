#ifndef WORLD_HPP
#define WORLD_HPP

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>

#include <array>
#include "Landscape.hpp"
#include "Player.hpp"

struct World : ftxui::ComponentBase
{
  static constexpr std::size_t HallCount{8};

  World(
    std::size_t width, 
    std::size_t height, 
    std::size_t& initialHallIndex,
    std::size_t bottomPanelSize, 
    const std::array<std::string, HallCount>& hallPaths,
    std::function<void()> quitGameCallback);

  void NextIteration();

  ftxui::Element Render() override;
  bool OnEvent(ftxui::Event) override;
  
private:
  std::size_t width_;
  std::size_t height_;
  std::size_t& currentHallIndex_;

  std::shared_ptr<Landscape> landscape_;

  std::array<std::shared_ptr<Bitmap>, HallCount> halls_;
  std::shared_ptr<Bitmap> currentFrame_;

  Player player_;
  Box worldBoundaryBox_;

  std::function<void()> quitGameCallback_;
};

#endif