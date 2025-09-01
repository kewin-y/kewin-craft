#include "chunk.hpp"
#include "FastNoiseLite.h"
#include <cstdint>
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <iostream>

// clang-format off
static constexpr uint8_t face_nz[][3] = {
        {0, 0, 0},
        {1, 0, 0},
        {1, 1, 0},
        {1, 1, 0},
        {0, 1, 0},
        {0, 0, 0},
};
static constexpr uint8_t face_pz[][3] = {
        {0, 0, 1},
        {1, 0, 1},
        {1, 1, 1},
        {1, 1, 1},
        {0, 1, 1},
        {0, 0, 1},
};
static constexpr uint8_t face_nx[][3] = {
        {0, 1, 1},
        {0, 1, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 1},
        {0, 1, 1},
};
static constexpr uint8_t face_px[][3] = {
        {1, 1, 1},
        {1, 1, 0},
        {1, 0, 0},
        {1, 0, 0},
        {1, 0, 1},
        {1, 1, 1},
};
static constexpr uint8_t face_ny[][3] = {
        {0, 0, 0},
        {1, 0, 0},
        {1, 0, 1},
        {1, 0, 1},
        {0, 0, 1},
        {0, 0, 0},
};
static constexpr uint8_t face_py[][3] = {
        {0, 1, 0},
        {1, 1, 0},
        {1, 1, 1},
        {1, 1, 1},
        {0, 1, 1},
        {0, 1, 0},
};
static constexpr glm::vec3 normals_nz[] = {
        {0.0f,  0.0f, -1.0f},
        {0.0f,  0.0f, -1.0f},
        {0.0f,  0.0f, -1.0f},
        {0.0f,  0.0f, -1.0f},
        {0.0f,  0.0f, -1.0f},
        {0.0f,  0.0f, -1.0f},
};

static constexpr glm::vec3 normals_pz[] = {
        {0.0f,  0.0f,  1.0f},
        {0.0f,  0.0f,  1.0f},
        {0.0f,  0.0f,  1.0f},
        {0.0f,  0.0f,  1.0f},
        {0.0f,  0.0f,  1.0f},
        {0.0f,  0.0f,  1.0f},
};
static constexpr glm::vec3 normals_nx[] = {
        {-1.0f,  0.0f,  0.0f},
        {-1.0f,  0.0f,  0.0f},
        {-1.0f,  0.0f,  0.0f},
        {-1.0f,  0.0f,  0.0f},
        {-1.0f,  0.0f,  0.0f},
        {-1.0f,  0.0f,  0.0f},
};
static constexpr glm::vec3 normals_px[] = {
        {1.0f,  0.0f,  0.0f},
        {1.0f,  0.0f,  0.0f},
        {1.0f,  0.0f,  0.0f},
        {1.0f,  0.0f,  0.0f},
        {1.0f,  0.0f,  0.0f},
        {1.0f,  0.0f,  0.0f},
};
static constexpr glm::vec3 normals_ny[] = {
        {0.0f, -1.0f,  0.0f},
        {0.0f, -1.0f,  0.0f},
        {0.0f, -1.0f,  0.0f},
        {0.0f, -1.0f,  0.0f},
        {0.0f, -1.0f,  0.0f},
        {0.0f, -1.0f,  0.0f},
};
static constexpr glm::vec3 normals_py[] = {
        {0.0f,  1.0f,  0.0f},
        {0.0f,  1.0f,  0.0f},
        {0.0f,  1.0f,  0.0f},
        {0.0f,  1.0f,  0.0f},
        {0.0f,  1.0f,  0.0f},
        {0.0f,  1.0f,  0.0f},
};
static constexpr glm::vec2 uvs[] = {
        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f},
        {1.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 1.0f},
};
// clang-format on

namespace kwnc
{
Chunk::Chunk(int x, int y, int z)
    : chunk_x{x}, chunk_y{y}, chunk_z{z}, vertex_array{}
{
}

Chunk::~Chunk()
{
        if (nullptr == blocks)
                return;

        delete[] blocks;
}

bool Chunk::is_empty() { return nullptr == blocks; }

bool Chunk::is_dirty() { return dirty; }

void Chunk::generate_terrain(const FastNoiseLite &noise)
{
        for (int x = 0; x < CHUNK_SIZE; ++x) {
                for (int z = 0; z < CHUNK_SIZE; ++z) {
                        int height = std::floor(
                            TERRAIN_CONSTANT *
                                noise.GetNoise(static_cast<float>(x) + chunk_x * CHUNK_SIZE,
                                               static_cast<float>(z) +
                                                   chunk_z * CHUNK_SIZE) +
                            TERRAIN_CONSTANT);

                        int local_height = glm::min(
                            CHUNK_SIZE,
                            glm::max(0, height - chunk_y * CHUNK_SIZE));

                        int y = 0;

                        for (; y < local_height; ++y) {
                                set_voxel(x, y, z, Block::DIRT);
                        }

                        for (; y < CHUNK_SIZE; ++y) {
                                set_voxel(x, y, z, Block::AIR);
                        }
                }
        }
}

void Chunk::set_voxel(int x, int y, int z, Block type)
{
        int idx;

        if (nullptr == blocks) {
                blocks = new Block[CHUNK_SIZE_CUBED];
        }

        idx = convert_to_block_idx(x, y, z);

        blocks[idx] = type;

        dirty = true;
}

void Chunk::fill()
{

        if (nullptr == blocks) {
                blocks = new Block[CHUNK_SIZE_CUBED];
        }

        for (int i = 0; i < CHUNK_SIZE_CUBED; i++) {
                blocks[i] = Block::DIRT;
        }

        dirty = true;
}

void Chunk::reset()
{
        if (nullptr == blocks)
                return;

        delete[] blocks;

        blocks = nullptr;
}

void Chunk::generate_mesh()
{
        if (nullptr == blocks)
                return;

        // NOTE: 0 0 0 is the top-left block on the most bottom layer
        Block_Vertex *vertex_data = new Block_Vertex[36 * CHUNK_SIZE_CUBED];
        Block_Vertex *current = vertex_data;

        int x, y, z;
        size_t vertex_count, size;

        for (int i = 0; i < CHUNK_SIZE_CUBED; ++i) {
                convert_to_pos_in_chunk(i, &x, &y, &z);

                if (blocks[i] == Block::AIR)
                        continue;

                auto block_type = static_cast<u_int8_t>(blocks[i]);

                // Negative Z
                if (z - 1 < 0 ||
                    blocks[convert_to_block_idx(x, y, z - 1)] == Block::AIR) {
                        for (int j = 0; j < 6; ++j) {
                                current->normal = normals_nz[j];
                                current->uv = get_uv(block_type, uvs[j]);
                                current->packed_coord_type =
                                    ((face_nz[j][0] + x) << 24u) |
                                    ((face_nz[j][1] + y) << 16u) |
                                    ((face_nz[j][2] + z) << 8u) | block_type;
                                current++;
                        }
                }
                // Positive Z
                if (z + 1 > 31 ||
                    blocks[convert_to_block_idx(x, y, z + 1)] == Block::AIR) {
                        for (int j = 0; j < 6; ++j) {
                                current->normal = normals_pz[j];
                                current->uv = get_uv(block_type, uvs[j]);
                                current->packed_coord_type =
                                    ((face_pz[j][0] + x) << 24u) |
                                    ((face_pz[j][1] + y) << 16u) |
                                    ((face_pz[j][2] + z) << 8u) | block_type;
                                current++;
                        }
                }
                // Negative X
                if (x - 1 < 0 ||
                    blocks[convert_to_block_idx(x - 1, y, z)] == Block::AIR) {
                        for (int j = 0; j < 6; ++j) {
                                current->normal = normals_nx[j];
                                current->uv = get_uv(block_type, uvs[j]);
                                current->packed_coord_type =
                                    ((face_nx[j][0] + x) << 24u) |
                                    ((face_nx[j][1] + y) << 16u) |
                                    ((face_nx[j][2] + z) << 8u) | block_type;
                                current++;
                        }
                }
                // Positive X
                if (x + 1 > 31 ||
                    blocks[convert_to_block_idx(x + 1, y, z)] == Block::AIR) {
                        for (int j = 0; j < 6; ++j) {
                                current->normal = normals_px[j];
                                current->uv = get_uv(block_type, uvs[j]);
                                current->packed_coord_type =
                                    ((face_px[j][0] + x) << 24u) |
                                    ((face_px[j][1] + y) << 16u) |
                                    ((face_px[j][2] + z) << 8u) | block_type;
                                current++;
                        }
                }
                // Negative Y
                if (y - 1 < 0 ||
                    blocks[convert_to_block_idx(x, y - 1, z)] == Block::AIR) {
                        for (int j = 0; j < 6; ++j) {
                                current->normal = normals_ny[j];
                                current->uv = get_uv(block_type, uvs[j]);
                                current->packed_coord_type =
                                    ((face_ny[j][0] + x) << 24u) |
                                    ((face_ny[j][1] + y) << 16u) |
                                    ((face_ny[j][2] + z) << 8u) | block_type;
                                current++;
                        }
                }
                // Positive Y
                if (y + 1 > 31 ||
                    blocks[convert_to_block_idx(x, y + 1, z)] == Block::AIR) {
                        for (int j = 0; j < 6; ++j) {
                                current->normal = normals_py[j];
                                current->uv = get_uv(block_type, uvs[j]);
                                current->packed_coord_type =
                                    ((face_py[j][0] + x) << 24u) |
                                    ((face_py[j][1] + y) << 16u) |
                                    ((face_py[j][2] + z) << 8u) | block_type;
                                current++;
                        }
                }
        }

        vertex_count = current - vertex_data;
        size = vertex_count * sizeof(Block_Vertex);
        vertex_array.buffer_data(size, vertex_count, vertex_data);

        dirty = false;
        delete[] vertex_data;
}

int Chunk::convert_to_block_idx(int x, int y, int z)
{
        return z + y * CHUNK_SIZE + x * CHUNK_SIZE_SQUARED;
}

void Chunk::convert_to_pos_in_chunk(int i, int *x, int *y, int *z)
{
        *x = i / CHUNK_SIZE_SQUARED;
        *y = (i / CHUNK_SIZE) % CHUNK_SIZE;
        *z = i % CHUNK_SIZE;
}
glm::vec2 Chunk::get_uv(u_int8_t type, const glm::vec2 &local_uv)
{
        float tile_size = 1.0f / 16.0f;
        u_int8_t tile_x = type % 16;
        u_int8_t tile_y = type / 16;
        return glm::vec2(tile_size * (tile_x + local_uv.x),
                         tile_size * (tile_y + local_uv.y));
}
} // namespace kwnc
