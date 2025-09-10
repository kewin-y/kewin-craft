#include "map.hpp"
#include "FastNoiseLite.h"
#include "chunk.hpp"
#include <atomic>
#include <cstdlib>
#include <future>
#include <glm/common.hpp>
#include <iostream>
#include <memory>
#include <mutex>
#include <tuple>
#include <vector>

static constexpr int RENDER_RADIUS = 3;
static constexpr int RENDER_DIAMETER = 2 * RENDER_RADIUS + 1;
static constexpr int MAX_LOADED_CHUNKS =
    RENDER_DIAMETER * RENDER_DIAMETER * RENDER_DIAMETER;

namespace kwnc
{
size_t
chunk_coordinate_hash::operator()(const std::tuple<int, int, int> &coords) const
{
        return (std::hash<int>{}(std::get<0>(coords))) ^
               ((std::hash<int>{}(std::get<1>(coords))) ^
                (std::hash<int>{}(std::get<2>(coords)) << 1) << 1);
}
Map::Map() : noise{}
{
        chunks.reserve(MAX_LOADED_CHUNKS);
        noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        // TODO: Generate Seed
        noise.SetSeed(0);
}

static std::mutex s_chunks_mtx;

static void init_chunks(
    std::unordered_map<std::tuple<int, int, int>, std::shared_ptr<Chunk>,
                       chunk_coordinate_hash> &chunks,
    const FastNoiseLite &noise, int x, int y, int z)
{
        auto key = std::make_tuple(x, y, z);
        auto chunk = std::make_shared<Chunk>(x, y, z);
        chunk->generate_terrain(noise);
        std::lock_guard<std::mutex> lock(s_chunks_mtx);
        chunks.emplace(key, chunk);
}

#define MULTITHREADING

void Map::setup(const glm::vec3 &camera_position)
{
        int camera_chunk_x, camera_chunk_y, camera_chunk_z;
        int start_x, start_y, start_z;

#ifdef MULTITHREADING
        std::vector<std::future<void>> futures;
        futures.reserve(MAX_LOADED_CHUNKS);
#endif // MULTITHREADING

        camera_chunk_x = std::floor(camera_position.x / Chunk::CHUNK_SIZE);
        camera_chunk_y = std::floor(camera_position.y / Chunk::CHUNK_SIZE);
        camera_chunk_z = std::floor(camera_position.z / Chunk::CHUNK_SIZE);

        start_x = camera_chunk_x - RENDER_RADIUS;
        start_y = camera_chunk_y - RENDER_RADIUS;
        start_z = camera_chunk_z - RENDER_RADIUS;

        for (int z = start_z; z < RENDER_DIAMETER + start_z; z++) {
                for (int y = start_y; y < RENDER_DIAMETER + start_y; y++) {
                        for (int x = start_x; x < RENDER_DIAMETER + start_x;
                             x++) {
#ifdef MULTITHREADING
                                futures.emplace_back(
                                    std::async(std::launch::async, init_chunks,
                                               std::ref(chunks),
                                               std::ref(noise), x, y, z));
#else
                                auto key = std::make_tuple(x, y, z);
                                auto chunk = std::make_shared<Chunk>(x, y, z);
                                chunk->generate_terrain(noise);
                                std::lock_guard<std::mutex> lock(s_chunks_mtx);
                                chunks.emplace(key, chunk);
#endif
                        }
                }
        }
#ifdef MULTITHREADING
        for (auto &future : futures) {
                future.get();
        }
#endif

        last_camera_chunk_x = camera_chunk_x;
        last_camera_chunk_y = camera_chunk_y;
        last_camera_chunk_z = camera_chunk_z;
}

void Map::update(const glm::vec3 &camera_position)
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

void Map::render(const Shader &shader)
{
        for (auto &[pos, chunk] : chunks) {
                if (chunk->dirty.load(std::memory_order_seq_cst)) {
                        chunk->generate_mesh();
                }

                auto world_pos = glm::vec3{Chunk::CHUNK_SIZE * chunk->chunk_x,
                                           Chunk::CHUNK_SIZE * chunk->chunk_y,
                                           Chunk::CHUNK_SIZE * chunk->chunk_z};
                shader.uniform_v3("world_pos", &world_pos[0]);
                chunk->vertex_array.draw();
        }
}

void Map::new_chunks_x(int dx, int camera_chunk_x, int camera_chunk_y,
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

        for (int z = start_z; z < RENDER_DIAMETER + start_z; z++) {
                for (int y = start_y; y < RENDER_DIAMETER + start_y; y++) {
                        auto removed = std::make_tuple(removed_edge, y, z);

                        chunks.erase(removed);

                        auto added = std::make_tuple(added_edge, y, z);

                        if (!chunks.count(added)) {
                                auto chunk =
                                    std::make_shared<Chunk>(added_edge, y, z);

                                chunk->generate_terrain(noise);

                                chunks.insert({added, chunk});
                        }
                }
        }
}

void Map::new_chunks_y(int dy, int camera_chunk_x, int camera_chunk_y,
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

        for (int z = start_z; z < RENDER_DIAMETER + start_z; z++) {
                for (int x = start_x; x < RENDER_DIAMETER + start_x; x++) {
                        auto removed = std::make_tuple(x, removed_edge, z);

                        chunks.erase(removed);

                        auto added = std::make_tuple(x, added_edge, z);

                        if (!chunks.count(added)) {
                                auto chunk =
                                    std::make_shared<Chunk>(x, added_edge, z);

                                chunk->generate_terrain(noise);

                                chunks.insert({added, chunk});
                        }
                }
        }
}

void Map::new_chunks_z(int dz, int camera_chunk_x, int camera_chunk_y,
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

        for (int y = start_y; y < RENDER_DIAMETER + start_y; y++) {
                for (int x = start_x; x < RENDER_DIAMETER + start_x; x++) {
                        auto removed = std::make_tuple(x, y, removed_edge);

                        chunks.erase(removed);

                        auto added = std::make_tuple(x, y, added_edge);

                        if (!chunks.count(added)) {
                                auto chunk =
                                    std::make_shared<Chunk>(x, y, added_edge);

                                chunk->generate_terrain(noise);

                                chunks.insert({added, chunk});
                        }
                }
        }
}
} // namespace kwnc
