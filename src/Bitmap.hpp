#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <cinttypes>
#include <vector>
#include <string_view>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Pixel
{
  bool state{false};
  glm::u8vec3 color;
};

struct Bitmap
{
  Bitmap(std::size_t width, std::size_t height)
    :  width_(width), height_(height), pixels_(width * height, Pixel{})
  {}

  explicit Bitmap(const std::string& imgPath);

  void DrawOther(std::size_t x0, std::size_t y0, const Bitmap& other);
  void DrawLine(glm::ivec2 p00, glm::ivec2 p11, const std::vector<glm::u8vec3>& colors);
  void Clear();

  [[nodiscard]] auto&       at(std::size_t index)                  { return pixels_.at(index); }
  [[nodiscard]] auto&       at(std::size_t x, std::size_t y)       { return pixels_.at(y * width_ + x); }
  [[nodiscard]] const auto& at(std::size_t x, std::size_t y) const { return pixels_.at(y * width_ + x); }

  [[nodiscard]] auto width() const noexcept { return width_; }
  [[nodiscard]] auto height() const noexcept { return height_; }

private:
  std::size_t width_;
  std::size_t height_;

  std::vector<Pixel> pixels_;
};

#endif