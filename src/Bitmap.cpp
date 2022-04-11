#include "Bitmap.hpp"

#include <lodepng.h>
#include <vector>
#include <cassert>

Bitmap::Bitmap(const std::string& imgPath)
{
  std::vector<unsigned char> img;
  unsigned int width{0U};
  unsigned int height{0U};

  /*[[maybe_unused]] const auto err =*/ lodepng::decode(img, width, height, imgPath);

  //assert(err == 0); // DEBUG

  width_ = width;
  height_ = height;

  pixels_.resize(width_ * height_);

  std::size_t imgIter{0};
  
  for(std::size_t y{0}; y<height_; ++y)
  {
    for(std::size_t x{0}; x<width_; ++x)
    {
      auto& px = at(x, y);

      px.color[0] = img[imgIter++];
      px.color[1] = img[imgIter++];
      px.color[2] = img[imgIter++];

      if(img[imgIter++] > 0)
      {
        px.state = true;
      }
    }
  }
}

void Bitmap::DrawOther(std::size_t x0, std::size_t y0, const Bitmap& other) 
{
  const auto x1 = x0 + other.width_;
  const auto y1 = y0 + other.height_;

  //assert(x1 <= width_ && y1 <= height_);// DEBUG

  // m = this bitmap, n = other bitmap
  for(std::size_t y_m{y0}, y_n{0}; y_m<y1 && y_n < other.height_; ++y_m, ++y_n)
  {
    for(std::size_t x_m{x0}, x_n{0}; x_m<x1 && x_n < other.width_; ++x_m, ++x_n)
    {
      this->at(x_m, y_m) = other.at(x_n, y_n);
    }
  }
}

void Bitmap::Clear()
{
  for(auto& pixel : pixels_)
  {
    pixel.state = false;
  }
}

/// based on ftxui canvas draw line implementation
void Bitmap::DrawLine(glm::ivec2 p00, glm::ivec2 p11, const std::vector<glm::u8vec3>& colors) 
{
  const auto dx = std::abs(p11[0] - p00[0]);
  const auto dy = std::abs(p11[1] - p00[1]);
  const auto sx = p11[0] > p00[0] ? 1 : -1;
  const auto sy = p11[1] > p00[1] ? 1 : -1;
  const auto len = std::max(dx, dy);

  if(dx * dy > static_cast<int32_t>(pixels_.size()))
  {
    return;
  }

  auto error = dx - dy;
  for (int32_t i{0}, x{p00[0]}, y{p00[1]}; 
        i < len && 
        ( x >= 0 && x < static_cast<int32_t>(width_) ) &&
        ( y >= 0 && y < static_cast<int32_t>(height_)) ; 
        ++i) 
  {
    auto& px = at(static_cast<std::size_t>(x), static_cast<std::size_t>(y));
    px.state = true;
    px.color = colors.at(static_cast<std::size_t>(i) % colors.size());

    if (error >= -dy) 
    {
      error -= dy;
      x += sx;
    }
    if (error <= dx) 
    {
      error += dx;
      y += sy;
    }
  }
}
