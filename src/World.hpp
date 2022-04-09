#ifndef WORLD_HPP
#define WORLD_HPP

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>

#include "CollisionValidator.hpp"
#include "Landscape.hpp"
#include "Player.hpp"

struct World : ftxui::ComponentBase
{
  World(
    std::size_t width, 
    std::size_t height, 
    std::size_t& initialHallIndex,
    const std::array<std::string, 8>& hallPaths,
    std::function<void()> quitGameCallback);

  void NextIteration();

  ftxui::Element Render() override;
  bool OnEvent(ftxui::Event) override;
  
private:
  std::size_t width_;
  std::size_t height_;
  std::size_t& currentHallIndex_;

  std::shared_ptr<Landscape> landscape_;

  std::array<std::shared_ptr<Bitmap>, 8> halls_;
  std::shared_ptr<Bitmap> currentFrame_;

  Player player_;
  CollisionValidator collisionValidator_;

  std::function<void()> quitGameCallback_;
};

#endif