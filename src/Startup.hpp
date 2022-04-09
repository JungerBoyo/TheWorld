#ifndef STARTUP_HPP
#define STARTUP_HPP

#include <ftxui/component/component.hpp>

struct Startup : ftxui::ComponentBase
{
  Startup(std::string_view startupTextDataJsonPath, std::size_t& startupOut, const std::function<void()>& contextSwitchCallback);

  ftxui::Element Render() override;

private:
  ftxui::Component lhsButton_;
  ftxui::Component rhsButton_;

  ftxui::Element circleCanv_;
  ftxui::Element squareCanv_;

  struct StartupSubstageInfo
  {
    std::string lhsButtonText_;
    std::string rhsButtonText_;

    std::array<std::string, 3> textLines_;
  };

  static constexpr std::size_t answerStageIndex { 3UL };
  static constexpr std::size_t lookStageIndex   { 4UL };
  static constexpr std::size_t imStageIndex     { 5UL };
  static constexpr std::pair<std::size_t, std::size_t> haveStageIndices{ 6UL, 7UL };
  std::array<StartupSubstageInfo, 8> substageInfos_;

  std::string currentLhsButtonText_;
  std::string currentRhsButtonText_;

  std::size_t currentSubstageIndex_{ 0 };
};

#endif