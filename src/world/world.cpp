#include "world/world.hpp"
#include "FastNoiseLite.h"
#include "world/chunk.hpp"
#include "world/chunk_coordinate.hpp"
#include <future>
#include <glm/common.hpp>
#include <memory>
#include <mutex>
#include <vector>


namespace kwnc
{
World::World() : noise{}
{
  chunks.reserve(MAX_LOADED_CHUNKS);
  noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

  // TODO: Generate Seed
  noise.SetSeed(0);
}

void World::setup(const glm::vec3 &camera_position)
{
  int camera_chunk_x, camera_chunk_y, camera_chunk_z;
  int start_x, start_y, start_z;

  std::vector<std::future<void>> futures;
  futures.reserve(MAX_LOADED_CHUNKS);

  camera_chunk_x = std::floor(camera_position.x / Chunk::CHUNK_SIZE);
  camera_chunk_y = std::floor(camera_position.y / Chunk::CHUNK_SIZE);
  camera_chunk_z = std::floor(camera_position.z / Chunk::CHUNK_SIZE);

  start_x = camera_chunk_x - RENDER_RADIUS;
  start_y = camera_chunk_y - RENDER_RADIUS;
  start_z = camera_chunk_z - RENDER_RADIUS;

  last_camera_chunk_x = camera_chunk_x;
  last_camera_chunk_y = camera_chunk_y;
  last_camera_chunk_z = camera_chunk_z;

  for (int z = start_z; z < RENDER_DIAMETER + start_z; z++) {
    for (int y = start_y; y < RENDER_DIAMETER + start_y; y++) {
      for (int x = start_x; x < RENDER_DIAMETER + start_x; x++) {

        auto init_chunk = [this, x, y, z]() {
          auto key = Chunk_Coordinate{x, y, z};
          auto chunk = std::make_shared<Chunk>(x, y, z);
          chunk->generate_terrain(this->noise);

          std::scoped_lock<std::mutex> lock(this->mutex);
          this->chunks.emplace(key, chunk);
        };

        futures.emplace_back(std::async(std::launch::async, init_chunk));
      }
    }
  }

  for (auto &future : futures) {
    future.get();
  }
}

void World::update(const glm::vec3 &camera_position)
{
  int camera_chunk_x, camera_chunk_y, camera_chunk_z;
  int dx, dy, dz;

  camera_chunk_x = std::floor(camera_position.x / Chunk::CHUNK_SIZE);
  camera_chunk_y = std::floor(camera_position.y / Chunk::CHUNK_SIZE);
  camera_chunk_z = std::floor(camera_position.z / Chunk::CHUNK_SIZE);

  if (camera_chunk_x == last_camera_chunk_x &&
      camera_chunk_y == last_camera_chunk_y &&
      camera_chunk_z == last_camera_chunk_z) {
    return;
  }

  dx = camera_chunk_x - last_camera_chunk_x;
  dy = camera_chunk_y - last_camera_chunk_y;
  dz = camera_chunk_z - last_camera_chunk_z;

  new_chunks_x(dx, camera_chunk_x, camera_chunk_y, camera_chunk_z);
  new_chunks_y(dy, camera_chunk_x, camera_chunk_y, camera_chunk_z);
  new_chunks_z(dz, camera_chunk_x, camera_chunk_y, camera_chunk_z);

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

void World::new_chunks_x(int dx, int camera_chunk_x, int camera_chunk_y,
                       int camera_chunk_z)
{
  if (dx == 0)
    return;

  int start_y = camera_chunk_y - RENDER_RADIUS;
  int start_z = camera_chunk_z - RENDER_RADIUS;

  int added_edge = (dx > 0) ? camera_chunk_x + RENDER_RADIUS
                            : camera_chunk_x - RENDER_RADIUS;
  int removed_edge = (dx > 0) ? last_camera_chunk_x - RENDER_RADIUS
                              : last_camera_chunk_x + RENDER_RADIUS;

  std::vector<std::future<void>> futures;
  futures.reserve(RENDER_DIAMETER * RENDER_DIAMETER);

  for (int z = start_z; z < RENDER_DIAMETER + start_z; z++) {
    for (int y = start_y; y < RENDER_DIAMETER + start_y; y++) {
      auto move_chunks = [this, added_edge, removed_edge, z, y]() {
        auto added = Chunk_Coordinate{added_edge, y, z};
        auto removed = Chunk_Coordinate{removed_edge, y, z};

        std::scoped_lock<std::mutex> lock(this->mutex);

        chunks.erase(removed);

        if (!chunks.count(added)) {
          auto chunk = std::make_shared<Chunk>(added_edge, y, z);

          chunk->generate_terrain(noise);

          chunks.insert({added, chunk});
        }
      };

      futures.emplace_back(std::async(std::launch::async, move_chunks));
    }
  }

  for (auto &future : futures) {
    future.get();
  }
}

void World::new_chunks_y(int dy, int camera_chunk_x, int camera_chunk_y,
                       int camera_chunk_z)
{
  if (dy == 0)
    return;

  int start_x = camera_chunk_x - RENDER_RADIUS;
  int start_z = camera_chunk_z - RENDER_RADIUS;

  int added_edge = (dy > 0) ? camera_chunk_y + RENDER_RADIUS
                            : camera_chunk_y - RENDER_RADIUS;
  int removed_edge = (dy > 0) ? last_camera_chunk_y - RENDER_RADIUS
                              : last_camera_chunk_y + RENDER_RADIUS;

  std::vector<std::future<void>> futures;
  futures.reserve(RENDER_DIAMETER * RENDER_DIAMETER);

  for (int z = start_z; z < RENDER_DIAMETER + start_z; z++) {
    for (int x = start_x; x < RENDER_DIAMETER + start_x; x++) {
      auto move_chunks = [this, added_edge, removed_edge, z, x]() {
        auto added = Chunk_Coordinate{x, added_edge, z};
        auto removed = Chunk_Coordinate{x, removed_edge, z};

        std::scoped_lock<std::mutex> lock(this->mutex);

        chunks.erase(removed);

        if (!chunks.count(added)) {
          auto chunk = std::make_shared<Chunk>(x, added_edge, z);

          chunk->generate_terrain(noise);

          chunks.insert({added, chunk});
        }
      };

      futures.emplace_back(std::async(std::launch::async, move_chunks));
    }
  }

  for (auto &future : futures) {
    future.get();
  }
}

void World::new_chunks_z(int dz, int camera_chunk_x, int camera_chunk_y,
                       int camera_chunk_z)
{
  if (dz == 0)
    return;

  int start_x = camera_chunk_x - RENDER_RADIUS;
  int start_y = camera_chunk_y - RENDER_RADIUS;

  int added_edge = (dz > 0) ? camera_chunk_z + RENDER_RADIUS
                            : camera_chunk_z - RENDER_RADIUS;
  int removed_edge = (dz > 0) ? last_camera_chunk_z - RENDER_RADIUS
                              : last_camera_chunk_z + RENDER_RADIUS;

  std::vector<std::future<void>> futures;
  futures.reserve(RENDER_DIAMETER * RENDER_DIAMETER);

  for (int y = start_y; y < RENDER_DIAMETER + start_y; y++) {
    for (int x = start_x; x < RENDER_DIAMETER + start_x; x++) {
      auto move_chunks = [this, added_edge, removed_edge, y, x]() {
        auto added = Chunk_Coordinate{x, y, added_edge};
        auto removed = Chunk_Coordinate{x, y, removed_edge};

        std::scoped_lock<std::mutex> lock(this->mutex);

        chunks.erase(removed);

        if (!chunks.count(added)) {
          auto chunk = std::make_shared<Chunk>(x, y, added_edge);

          chunk->generate_terrain(noise);

          chunks.insert({added, chunk});
        }
      };

      futures.emplace_back(std::async(std::launch::async, move_chunks));
    }
  }

  for (auto &future : futures) {
    future.get();
  }
}
} // namespace kwnc
