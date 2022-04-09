#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <thread>
#include <ftxui/component/screen_interactive.hpp>

struct Screen
{
  Screen(int32_t width, int32_t height, double fps);

  void Draw(ftxui::Component component);

  auto ExitClosure() { return nativeScr_.ExitLoopClosure(); }

  void TurnOnRefresher();
  void TurnOffRefresher();

  [[nodiscard]] auto width() const  { return width_; }
  [[nodiscard]] auto height() const { return height_; }

private:  
  int32_t width_;
  int32_t height_;
  double fps_;

  ftxui::ScreenInteractive nativeScr_;
  std::atomic<bool> loopEnd_;
  std::thread refresherThread_;
};

#endif