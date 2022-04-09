#include "Bitmap.hpp"

#include <lodepng.h>
#include <vector>
#include <cassert>

Bitmap::Bitmap(const std::string& imgPath)
{
  std::vector<unsigned char> img;
  unsigned int width{0U};
  unsigned int height{0U};

  [[maybe_unused]] const auto err = lodepng::decode(img, width, height, imgPath);

  assert(err == 0);

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

      if(img[imgIter++] == 0)
      {
        px.ch = U' ';
      }
      else
      {
        px.ch = U'█';
      }
    }
  }
}

void Bitmap::DrawOther(std::size_t x0, std::size_t y0, const Bitmap& other) 
{
  const auto x1 = x0 + other.width_;
  const auto y1 = y0 + other.height_;

  assert(x1 <= width_ && y1 <= height_);

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
    pixel.ch = U' ';
  }
}