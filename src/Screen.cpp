#include "Screen.hpp"

Screen::Screen(int32_t width, int32_t height, double fps)// NOLINT
  : 
    width_(width), 
    height_(height), 
    fps_(fps),
    nativeScr_(ftxui::ScreenInteractive::FixedSize(width, height))
{}

void Screen::TurnOnRefresher() noexcept
{
  loopEnd_ = false;
  refresherThread_ = std::thread([this]{
    while(!loopEnd_)
    {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(1.0s / fps_);// NOLINT time of one frame = 1sec / frames per second
      nativeScr_.PostEvent(ftxui::Event::Custom);
    }
  });
}

void Screen::TurnOffRefresher() noexcept
{
  loopEnd_ = true;
  if(refresherThread_.joinable())
  { 
    refresherThread_.join();
  }
}

void Screen::Draw(ftxui::Component component) noexcept
{
  nativeScr_.Loop(std::move(component));
}



