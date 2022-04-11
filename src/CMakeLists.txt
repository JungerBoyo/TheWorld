find_package(fmt CONFIG)
find_package(spdlog CONFIG)
find_package(docopt CONFIG)
find_package(nlohmann_json CONFIG)
find_package(lodepng CONFIG)
find_package(glm CONFIG)

include_directories(../)

# Generic test that uses conan libs
add_executable(TheWorld 
  main.cpp

  Screen.hpp
  Screen.cpp

  Startup.cpp
  Startup.hpp

  World.hpp 
  World.cpp

  Bitmap.hpp
  Bitmap.cpp

  Landscape.hpp
  Landscape.cpp

  Entity.hpp
  Entity.cpp

  Player.hpp
  Player.cpp

  AnimatedMovePattern.hpp
  AnimatedMovePattern.cpp

  Skill.hpp
  Skill.cpp

  Box.hpp
  Box.cpp
  Line.hpp
  Line.cpp 

  Boss.hpp
  Boss.cpp

  Hall.hpp
  Hall.cpp
) 


target_link_libraries(
  TheWorld
  PRIVATE project_options
          project_warnings
          docopt::docopt
          fmt::fmt
          spdlog::spdlog
          nlohmann_json::nlohmann_json
          lodepng::lodepng
          glm::glm)

target_link_system_libraries(
  TheWorld
  PRIVATE
  ftxui::screen
  ftxui::dom
  ftxui::component)

target_include_directories(TheWorld PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
