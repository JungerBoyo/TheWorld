#include "Hall.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

Hall::Hall(
  std::string_view hallConfigPath, 
  glm::ivec2 bossPosition, 
  glm::ivec2 respawnLowerRestriction,
  glm::ivec2 respawnUpperRestriction,
  const std::shared_ptr<Entity>& player) 
{
  std::ifstream stream(hallConfigPath.data());

  nlohmann::json j;
  stream >> j;
  stream.close();

  map = std::make_shared<Bitmap>(j["mapPath"].get<std::string>());

  const auto& jboss = j["boss"];
  const auto extent = jboss["extent"].get<std::pair<int32_t, int32_t>>();
  const auto dmgCooldown = jboss["dmgCooldown"].get<int32_t>();
  const auto health = jboss["health"].get<int32_t>();
  const auto rngSeed = jboss["randomSeed"].get<int32_t>();
  const auto name = jboss["name"].get<std::string>();

  boss = std::make_shared<Boss>(
    bossPosition, 
    glm::ivec2{extent.first, extent.second},
    dmgCooldown, 
    health, 
    name,
    respawnLowerRestriction,
    respawnUpperRestriction,
    rngSeed,
    player, 
    jboss["appearAnimPath"].get<std::string>(),
    jboss["disappearAnimPath"].get<std::string>(),
    jboss["targetAnimPath"].get<std::string>()
  );

}
