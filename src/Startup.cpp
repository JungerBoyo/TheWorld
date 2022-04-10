#include "Startup.hpp"

#include <nlohmann/json.hpp>
#include <fstream>

Startup::Startup(std::string_view startupTextDataJsonPath, std::size_t& startupOut, const std::function<void()>& contextSwitchCallback)
{
  /// loading text for substages from json file
  std::ifstream stream(startupTextDataJsonPath.data());

  //assert(stream.good());// DEBUG

  nlohmann::json j;
  stream >> j;
  stream.close();

  std::size_t i{ 0UL };
  for(const auto& value : j)
  {
    substageInfos_.at(i).lhsButtonText_ = value["lhsButtonText"].get<std::string>();
    substageInfos_.at(i).rhsButtonText_ = value["rhsButtonText"].get<std::string>();

    auto lines = value["textLines"].get<std::vector<std::string>>();

    substageInfos_.at(i).textLines_ = {{lines[0], lines[1], lines[2]}};

    ++i;
  }

  currentLhsButtonText_ = substageInfos_.front().lhsButtonText_;
  currentRhsButtonText_ = substageInfos_.front().rhsButtonText_;

  /// defining buttons
  startupOut = 0UL;

  lhsButton_ = ftxui::Button(&currentLhsButtonText_, 
  [&, contextSwitchCallback]{

    if(currentSubstageIndex_ >= lookStageIndex)
    {
      switch(currentSubstageIndex_)
      {
        case lookStageIndex: 
            ++currentSubstageIndex_;
            startupOut |= 0x1u;// NOLINT set bit 1
          break;

        case imStageIndex:
            currentSubstageIndex_ += 2;// NOLINT -> to "second have stage"
            startupOut |= 0x2u;// NOLINT set bit 2
          break;

        case haveStageIndices.first: case haveStageIndices.second:
            startupOut |= 0x4u;// NOLINT set bit 3
            contextSwitchCallback();
          break;

        default:
            ++currentSubstageIndex_;
          break;
      }
    }

    currentLhsButtonText_ = substageInfos_.at(currentSubstageIndex_).lhsButtonText_;
    currentRhsButtonText_ = substageInfos_.at(currentSubstageIndex_).rhsButtonText_;    
  });

  rhsButton_ = ftxui::Button(&currentRhsButtonText_, 
  [&, contextSwitchCallback]{

    if(currentSubstageIndex_ == haveStageIndices.first || currentSubstageIndex_ == haveStageIndices.second)
    {
      contextSwitchCallback();
    }
    else
    {
      ++currentSubstageIndex_;
    }

    currentLhsButtonText_ = substageInfos_.at(currentSubstageIndex_).lhsButtonText_;
    currentRhsButtonText_ = substageInfos_.at(currentSubstageIndex_).rhsButtonText_;   
  });

  this->Add(ftxui::Container::Vertical({lhsButton_, rhsButton_}));


  /// LHS and RHS images (circle and square)
  constexpr int32_t canvW{ 32 };
  constexpr int32_t canvH{ 32 };

  auto circle = ftxui::Canvas(canvW, canvH);
  circle.DrawBlockCircle(15, 15, 14);// NOLINT circle dims

  auto square = ftxui::Canvas(canvW, canvH);
  square.DrawBlockLine(0 , 0 , 28, 0 );// NOLINT upper line
  square.DrawBlockLine(28, 28, 0 , 28);// NOLINT bottom line
  square.DrawBlockLine(28, 0 , 28, 28);// NOLINT rhs line 
  square.DrawBlockLine(0 , 28, 0 , 0 );// NOLINT lhs line

  circleCanv_ = ftxui::canvas(circle);
  squareCanv_ = ftxui::canvas(square);
}

ftxui::Element Startup::Render()
{
  return ftxui::vbox({
          ftxui::filler(),
          ftxui::hbox({
            ftxui::filler(),
            circleCanv_,
            ftxui::filler(),
            ftxui::vbox({
              ftxui::text(substageInfos_.at(currentSubstageIndex_).textLines_.at(0)) | ftxui::bold | ftxui::center,// NOLINT 
              ftxui::text(substageInfos_.at(currentSubstageIndex_).textLines_.at(1)) | ftxui::bold | ftxui::center,// NOLINT
              ftxui::text(substageInfos_.at(currentSubstageIndex_).textLines_.at(2)) | ftxui::bold | ftxui::center // NOLINT
            }),    
            ftxui::filler(),
            squareCanv_,
            ftxui::filler()
          }),
          ftxui::filler(),
          (currentSubstageIndex_ > answerStageIndex) ?
             ftxui::hbox({lhsButton_->Render(), ftxui::filler(), rhsButton_->Render()}) :
             ftxui::hbox({ftxui::filler(), rhsButton_->Render()})
      }) | ftxui::border;
} 