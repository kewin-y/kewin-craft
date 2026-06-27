#include "world/world.hpp"
#include "FastNoiseLite.h"
#include "world/chunk.hpp"
#include "world/chunk_coordinate.hpp"
#include <glm/common.hpp>
#include <memory>
#include <unordered_set>

namespace kwnc
{
World::World() : noise{}
{
  chunks.reserve(MAX_LOADED_CHUNKS);
  noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

  // TODO: Generate Seed
  noise.SetSeed(0);
}

void World::update(const glm::vec3 &camera_position)
{
  using chunk_set =
      std::unordered_set<Chunk_Coordinate, Chunk_Coordinate_Hasher>;

  chunk_set s{};
  s.reserve(MAX_LOADED_CHUNKS);

  int camera_chunk_x, camera_chunk_y, camera_chunk_z;
  int start_x, start_y, start_z;

  camera_chunk_x = std::floor(camera_position.x / Chunk::CHUNK_SIZE);
  camera_chunk_y = std::floor(camera_position.y / Chunk::CHUNK_SIZE);
  camera_chunk_z = std::floor(camera_position.z / Chunk::CHUNK_SIZE);

  // Skip entire process if chunk position does not change
  if (camera_chunk_x == last_camera_chunk_x &&
      camera_chunk_y == last_camera_chunk_y &&
      camera_chunk_z == last_camera_chunk_z) {
    return;
  }

  start_x = camera_chunk_x - RENDER_RADIUS;
  start_y = camera_chunk_y - RENDER_RADIUS;
  start_z = camera_chunk_z - RENDER_RADIUS;

  for (int z = start_z; z < RENDER_DIAMETER + start_z; z++) {
    for (int y = start_y; y < RENDER_DIAMETER + start_y; y++) {
      for (int x = start_x; x < RENDER_DIAMETER + start_x; x++) {
        auto c = Chunk_Coordinate{x, y, z};
        s.insert(c);
      }
    }
  }

  for (auto it = chunks.begin(); it != chunks.end();) {
    if (!s.count(it->first)) {
      it = chunks.erase(it);
    } else {
      ++it;
    }
  }

  for (auto &chunk_coord : s) {
    if (!chunks.count(chunk_coord)) {
      auto chunk =
          std::make_shared<Chunk>(chunk_coord.x, chunk_coord.y, chunk_coord.z);

      chunk->generate_terrain(noise);

      chunks.insert({chunk_coord, chunk});
    }
  }

  last_camera_chunk_x = camera_chunk_x;
  last_camera_chunk_y = camera_chunk_y;
  last_camera_chunk_z = camera_chunk_z;
}

void World::render(const Shader &shader)
{

  for (auto &[pos, chunk] : chunks) {
    if (chunk->dirty) {
      chunk->generate_mesh();
    } else {
      auto world_pos = glm::vec3{Chunk::CHUNK_SIZE * chunk->chunk_x,
                                 Chunk::CHUNK_SIZE * chunk->chunk_y,
                                 Chunk::CHUNK_SIZE * chunk->chunk_z};
      shader.uniform_v3("world_pos", &world_pos[0]);
      chunk->vertex_array.draw();
    }
  }
}
} // namespace kwnc
