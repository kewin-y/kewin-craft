#ifndef WORLD_HPP
#define WORLD_HPP

#include "FastNoiseLite.h"
#include "graphics/shader.hpp"
#include "world/chunk.hpp"
#include "world/chunk_coordinate.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>

namespace kwnc
{

using Chunk_Map = std::unordered_map<Chunk_Coordinate, std::shared_ptr<Chunk>,
                                     Chunk_Coordinate_Hasher>;
class World
{
public:
  static constexpr int RENDER_RADIUS = 3;
  static constexpr int RENDER_DIAMETER = 2 * RENDER_RADIUS + 1;
  static constexpr int MAX_LOADED_CHUNKS =
      RENDER_DIAMETER * RENDER_DIAMETER * RENDER_DIAMETER;
  World();

  ~World() = default;

  void update(const glm::vec3 &camera_position);
  void render(const Shader &shader);

private:
  Chunk_Map chunks;

  int last_camera_chunk_x, last_camera_chunk_y, last_camera_chunk_z;

  FastNoiseLite noise;
};
} // namespace kwnc

#endif // !MAP_HPP
