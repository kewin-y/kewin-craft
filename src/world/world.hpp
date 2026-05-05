#ifndef MAP_HPP
#define MAP_HPP

#include "FastNoiseLite.h"
#include "graphics/shader.hpp"
#include "world/chunk.hpp"
#include "world/chunk_coordinate.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace kwnc
{

using Chunk_Map = std::unordered_map<Chunk_Coordinate, std::shared_ptr<Chunk>,
                                     Chunk_Coordinate_Hasher>;
class Map
{
public:
  Map();
  ~Map() = default;

  void setup(const glm::vec3 &camera_position);
  void update(const glm::vec3 &camera_position);
  void render(const Shader &shader);

private:
  Chunk_Map chunks;

  int last_camera_chunk_x, last_camera_chunk_y, last_camera_chunk_z;

  FastNoiseLite noise;

  mutable std::mutex mutex;

  void new_chunks_x(int dx, int camera_chunk_x, int camera_chunk_y,
                    int camera_chunk_z);
  void new_chunks_y(int dy, int camera_chunk_x, int camera_chunk_y,
                    int camera_chunk_z);
  void new_chunks_z(int dz, int camera_chunk_x, int camera_chunk_y,
                    int camera_chunk_z);

  static constexpr int RENDER_RADIUS = 3;
  static constexpr int RENDER_DIAMETER = 2 * RENDER_RADIUS + 1;
  static constexpr int MAX_LOADED_CHUNKS =
      RENDER_DIAMETER * RENDER_DIAMETER * RENDER_DIAMETER;
};
} // namespace kwnc

#endif // !MAP_HPP
