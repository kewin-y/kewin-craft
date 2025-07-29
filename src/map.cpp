#include "map.hpp"
#include "chunk.hpp"
#include <cstdlib>
#include <glm/common.hpp>
#include <iostream>

static constexpr int RENDER_RADIUS = 3;
static constexpr int RENDER_DIAMETER = 2 * (RENDER_RADIUS - 1) + 1;
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
Map::Map() { chunks.reserve(MAX_LOADED_CHUNKS); }

void Map::setup(const glm::vec3 &camera_position)
{
        int camera_chunk_x, camera_chunk_y, camera_chunk_z;
        int start_x, start_y, start_z;

        camera_chunk_x = std::floor(camera_position.x / Chunk::CHUNK_SIZE);
        camera_chunk_y = std::floor(camera_position.y / Chunk::CHUNK_SIZE);
        camera_chunk_z = std::floor(camera_position.z / Chunk::CHUNK_SIZE);

        start_x = camera_chunk_x - RENDER_RADIUS + 1;
        start_y = camera_chunk_y - RENDER_RADIUS + 1;
        start_z = camera_chunk_z - RENDER_RADIUS + 1;

        for (int z = start_z; z < RENDER_DIAMETER + start_z; z++) {
                for (int y = start_y; y < RENDER_DIAMETER + start_y; y++) {
                        for (int x = start_y; x < RENDER_DIAMETER + start_x;
                             x++) {
                                auto chunk = std::make_shared<Chunk>(x, y, z);

                                if (y <= 1)
                                        chunk->fill();

                                chunks.insert(
                                    {std::make_tuple(x, y, z), chunk});
                        }
                }
        }

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

        last_camera_chunk_x = camera_chunk_x;
        last_camera_chunk_y = camera_chunk_y;
        last_camera_chunk_z = camera_chunk_z;
}

void Map::render(const Shader &shader)
{
        for (auto &[pos, chunk] : chunks) {
                if (chunk->is_dirty())
                        chunk->generate_mesh();
                if (!chunk->is_empty()) {
                        auto world_pos =
                            glm::vec3{Chunk::CHUNK_SIZE * chunk->chunk_x,
                                      Chunk::CHUNK_SIZE * chunk->chunk_y,
                                      Chunk::CHUNK_SIZE * chunk->chunk_z};
                        shader.uniform_v3("world_pos", &world_pos[0]);
                        chunk->vertex_array.draw();
                }
        }
}
} // namespace kwnc
