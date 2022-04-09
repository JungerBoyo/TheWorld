#ifndef LANDSCAPE_HPP
#define LANDSCAPE_HPP

#include <cinttypes>
#include <cassert>
#include <ftxui/dom/node.hpp>

#include "Bitmap.hpp"

struct Landscape : ftxui::Node
{
  Landscape(std::size_t width, std::size_t height)
    : width_(width), height_(height)
  {}

  void SetBackground(std::shared_ptr<Bitmap> bmp);
  void SetForeground(std::shared_ptr<Bitmap> bmp);

  void ComputeRequirement() override;
  void Render(ftxui::Screen &scr) override;

  [[nodiscard]] auto width() const noexcept { return width_; }
  [[nodiscard]] auto height() const noexcept { return height_; }

private:
  std::size_t width_;
  std::size_t height_;

  std::shared_ptr<Bitmap> foregroundBmp_;
  std::shared_ptr<Bitmap> backgroundBmp_;
};

#endif