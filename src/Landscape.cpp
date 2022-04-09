#include "Landscape.hpp"


void Landscape::SetBackground(std::shared_ptr<Bitmap> bmp) 
{
  assert(bmp->width() == width_ && bmp->height() == height_);

  backgroundBmp_ = std::move(bmp);
}

void Landscape::SetForeground(std::shared_ptr<Bitmap> bmp)
{
  assert(bmp->width() == width_ && bmp->height() == height_);

  foregroundBmp_ = std::move(bmp);
}

void Landscape::ComputeRequirement() 
{
  requirement_ = ftxui::Requirement{
    .min_x = static_cast<int>(width_), .min_y = static_cast<int>(height_), .selected_box{ 0, 0, 0, 0 }
  };
}

void Landscape::Render(ftxui::Screen &scr) 
{
  assert(foregroundBmp_ && backgroundBmp_);

  for (std::size_t y{0}; y < height_; ++y) 
  {
    for (std::size_t x{0}; x < width_; ++x) 
    {
      auto& scrPx = scr.PixelAt(box_.x_min + static_cast<int>(x), box_.y_min + static_cast<int>(y)); 
      const auto index = y * width_ + x;

      const auto& bPx = backgroundBmp_->at(index);
      const auto& fPx = foregroundBmp_->at(index);

      //temporarly
      scrPx.character = "█";

      if(fPx.ch == U' ')
      {
        //scrPx.character = bPx.ch;

        const auto color = bPx.color;
        scrPx.foreground_color = ftxui::Color{ color[0], color[1], color[2] };
      }
      else
      {
        //scrPx.character = fPx.ch;

        const auto color = fPx.color;
        scrPx.foreground_color = ftxui::Color{ color[0], color[1], color[2] };
      }
    }
  }
}