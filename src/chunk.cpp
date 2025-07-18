#include "chunk.hpp"
#include "block.hpp"
#include "block_vertex_array.hpp"
#include "glad/gl.h"
#include <cstddef>
#include <cstdlib>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <sys/types.h>

// I promise I'll make this code better

// clang-format off

// Negative Z (NZ)
static const u_int8_t face_nz[][3] = {
        {0, 0, 0},
        {1, 0, 0},
        {1, 1, 0},
        {1, 1, 0},
        {0, 1, 0},
        {0, 0, 0}
};

// Positive Z (PZ)
static const u_int8_t face_pz[][3] = {
        {0, 0, 1},
        {1, 0, 1},
        {1, 1, 1},
        {1, 1, 1},
        {0, 1, 1},
        {0, 0, 1}
};

// Negative X (NX)
static const u_int8_t face_nx[][3] = {
        {0, 1, 1},
        {0, 1, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 1, 1},
        {0, 0, 1}
};

// Positive X (PX)
static const u_int8_t face_px[][3] = {
        {1, 1, 1},
        {1, 1, 0},
        {1, 0, 0},
        {1, 0, 0},
        {1, 0, 1},
        {1, 1, 1}
};

// Negative Y (NY)
static const u_int8_t face_ny[][3] = {
        {0, 0, 0},
        {1, 0, 0},
        {1, 0, 1},
        {1, 0, 1},
        {0, 0, 1},
        {0, 0, 0}
};

// Positive Y (PY)
static const u_int8_t face_py[][3] ={
        {0, 1, 0},
        {1, 1, 0},
        {1, 1, 1},
        {1, 1, 1},
        {0, 1, 1},
        {0, 1, 0}
};
// clang-format on

// clang-format off
static const glm::vec3 normals_nz[] = {
        {0.0f,  0.0f,  -1.0f},
        {0.0f,  0.0f,  -1.0f},
        {0.0f,  0.0f,  -1.0f},
        {0.0f,  0.0f,  -1.0f},
        {0.0f,  0.0f,  -1.0f},
        {0.0f,  0.0f,  -1.0f},
};

static const glm::vec3 normals_pz[] = {
        {0.0f,  0.0f,   1.0f},
        {0.0f,  0.0f,   1.0f},
        {0.0f,  0.0f,   1.0f},
        {0.0f,  0.0f,   1.0f},
        {0.0f,  0.0f,   1.0f},
        {0.0f,  0.0f,   1.0f},
};

static const glm::vec3 normals_nx[] = {
        {-1.0f,  0.0f,  0.0f},
        {-1.0f,  0.0f,  0.0f},
        {-1.0f,  0.0f,  0.0f},
        {-1.0f,  0.0f,  0.0f},
        {-1.0f,  0.0f,  0.0f},
        {-1.0f,  0.0f,  0.0f},
};

static const glm::vec3 normals_px[] = {
        {1.0f,  0.0f,  0.0f},
        {1.0f,  0.0f,  0.0f},
        {1.0f,  0.0f,  0.0f},
        {1.0f,  0.0f,  0.0f},
        {1.0f,  0.0f,  0.0f},
        {1.0f,  0.0f,  0.0f},
};

static const glm::vec3 normals_ny[] = {
        {0.0f, -1.0f,  0.0f},
        {0.0f, -1.0f,  0.0f},
        {0.0f, -1.0f,  0.0f},
        {0.0f, -1.0f,  0.0f},
        {0.0f, -1.0f,  0.0f},
        {0.0f, -1.0f,  0.0f},
};

static const glm::vec3 normals_py[] = {
        {0.0f,  1.0f,  0.0f},
        {0.0f,  1.0f,  0.0f},
        {0.0f,  1.0f,  0.0f},
        {0.0f,  1.0f,  0.0f},
        {0.0f,  1.0f,  0.0f},
        {0.0f,  1.0f,  0.0f}
};
// clang-format on

// clang-format off
static const glm::vec2 uvs[]  ={
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f}
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

void Chunk::add_voxel(int x, int y, int z, Block type)
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

        u_int8_t x, y, z;
        size_t vertex_count, size;

        for (int i = 0; i < CHUNK_SIZE_CUBED; ++i) {
                convert_to_pos_in_chunk(i, &x, &y, &z);

                if (blocks[i] == Block::AIR)
                        continue;

                // Negative Z
                if (z - 1 < 0 ||
                    blocks[convert_to_block_idx(x, y, z - 1)] == Block::AIR) {
                        for (int j = 0; j < 6; ++j) {
                                current->normal = normals_nz[j];
                                current->uv = uvs[j];
                                current->packed_coord_type =
                                    ((face_nz[j][0] + x) << 24u) |
                                    ((face_nz[j][1] + y) << 16u) |
                                    ((face_nz[j][2] + z) << 8u) |
                                    static_cast<u_int8_t>(blocks[i]);
                                current++;
                        }
                }
                // Positive Z
                if (z + 1 > 31 ||
                    blocks[convert_to_block_idx(x, y, z + 1)] == Block::AIR) {
                        for (int j = 0; j < 6; ++j) {
                                current->normal = normals_pz[j];
                                current->uv = uvs[j];
                                current->packed_coord_type =
                                    ((face_pz[j][0] + x) << 24u) |
                                    ((face_pz[j][1] + y) << 16u) |
                                    ((face_pz[j][2] + z) << 8u) |
                                    static_cast<u_int8_t>(blocks[i]);
                                current++;
                        }
                }
                // Negative X
                if (x - 1 < 0 ||
                    blocks[convert_to_block_idx(x - 1, y, z)] == Block::AIR) {
                        for (int j = 0; j < 6; ++j) {
                                current->normal = normals_nx[j];
                                current->uv = uvs[j];
                                current->packed_coord_type =
                                    ((face_nx[j][0] + x) << 24u) |
                                    ((face_nx[j][1] + y) << 16u) |
                                    ((face_nx[j][2] + z) << 8u) |
                                    static_cast<u_int8_t>(blocks[i]);
                                current++;
                        }
                }
                // Positive X
                if (x - 1 > 31 ||
                    blocks[convert_to_block_idx(x + 1, y, z)] == Block::AIR) {
                        for (int j = 0; j < 6; ++j) {
                                current->normal = normals_px[j];
                                current->uv = uvs[j];
                                current->packed_coord_type =
                                    ((face_px[j][0] + x) << 24u) |
                                    ((face_px[j][1] + y) << 16u) |
                                    ((face_px[j][2] + z) << 8u) |
                                    static_cast<u_int8_t>(blocks[i]);
                                current++;
                        }
                }
                // Negative Y
                if (y - 1 < 0 ||
                    blocks[convert_to_block_idx(x, y - 1, z)] == Block::AIR) {
                        for (int j = 0; j < 6; ++j) {
                                current->normal = normals_ny[j];
                                current->uv = uvs[j];
                                current->packed_coord_type =
                                    ((face_ny[j][0] + x) << 24u) |
                                    ((face_ny[j][1] + y) << 16u) |
                                    ((face_ny[j][2] + z) << 8u) |
                                    static_cast<u_int8_t>(blocks[i]);
                                current++;
                        }
                }
                // Positive Y
                if (y + 1 > 31 ||
                    blocks[convert_to_block_idx(x, y + 1, z)] == Block::AIR) {
                        for (int j = 0; j < 6; ++j) {
                                current->normal = normals_py[j];
                                current->uv = uvs[j];
                                current->packed_coord_type =
                                    ((face_py[j][0] + x) << 24u) |
                                    ((face_py[j][1] + y) << 16u) |
                                    ((face_py[j][2] + z) << 8u) |
                                    static_cast<u_int8_t>(blocks[i]);
                                current++;
                        }
                }
        }

        vertex_count = current - vertex_data;
        size = vertex_count * sizeof(Block_Vertex);
        vertex_array.buffer_data(size, vertex_count, vertex_data);
        std::cout << "There are: " << current - vertex_data << " vertices\n";

        dirty = false;
        delete[] vertex_data;
}

int Chunk::convert_to_block_idx(u_int8_t x, u_int8_t y, u_int8_t z)
{
        return z + y * CHUNK_SIZE + x * CHUNK_SIZE_SQUARED;
}

void Chunk::convert_to_pos_in_chunk(int i, u_int8_t *x, u_int8_t *y,
                                    u_int8_t *z)
{
        *z = i % CHUNK_SIZE;
        *y = (i % CHUNK_SIZE_SQUARED) / CHUNK_SIZE;
        *x = (i % CHUNK_SIZE_CUBED) / CHUNK_SIZE_SQUARED;
}
} // namespace kwnc
