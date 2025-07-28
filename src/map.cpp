#include "map.hpp"

static constexpr int render_radius = 1;
static constexpr int render_diameter = 2 * (render_radius - 1) + 1;
static constexpr int max_loaded_chunks =
    render_diameter * render_diameter * render_diameter;

namespace kwnc
{
size_t
chunk_coordinate_hash::operator()(const std::tuple<int, int, int> &coords) const
{
        return (std::hash<int>{}(std::get<0>(coords))) ^
               ((std::hash<int>{}(std::get<1>(coords))) ^
                (std::hash<int>{}(std::get<2>(coords)) << 1) << 1);
}
Map::Map() { chunks.reserve(max_loaded_chunks); }
// Map::~Map() {}

void Map::setup(const glm::vec3 camera_position)
{
        int camera_chunk_x, camera_chunk_y, camera_chunk_z;
        int start_x, start_y, start_z;

        camera_chunk_x = std::floor(camera_position.x / 32);
        camera_chunk_y = std::floor(camera_position.y / 32);
        camera_chunk_z = std::floor(camera_position.z / 32);

        start_x = camera_chunk_x - render_radius + 1;
        start_y = camera_chunk_y - render_radius + 1;
        start_z = camera_chunk_z - render_radius + 1;

        for (int z = start_z; z < render_diameter + start_z; z++) {
                for (int y = start_y; y < render_diameter + start_y; y++) {
                        for (int x = start_y; x < render_diameter + start_x;
                             x++) {
                                auto chunk = std::make_shared<Chunk>(x, y, z);

                                if (y <= 1)
                                        chunk->fill();

                                chunks.insert(
                                    {std::make_tuple(x, y, z), chunk});
                        }
                }
        }
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
