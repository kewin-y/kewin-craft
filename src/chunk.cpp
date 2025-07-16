#include "chunk.hpp"
#include "block.hpp"
#include "block_vertex_array.hpp"
#include "glad/gl.h"
#include <cstddef>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <iostream>

// I promise I'll make this code better

// clang-format off
static glm::vec3 face_nz[] = {
        // Negative Z (NZ)
        glm::vec3{-1.0f, -1.0f, -1.0f},
        glm::vec3{ 1.0f, -1.0f, -1.0f},
        glm::vec3{ 1.0f,  1.0f, -1.0f},
        glm::vec3{ 1.0f,  1.0f, -1.0f},
        glm::vec3{-1.0f,  1.0f, -1.0f},
        glm::vec3{-1.0f, -1.0f, -1.0f},
};

static glm::vec3 face_pz[] = {
        // Positive Z (PZ)
        glm::vec3{-1.0f, -1.0f,  1.0f},
        glm::vec3{ 1.0f, -1.0f,  1.0f},
        glm::vec3{ 1.0f,  1.0f,  1.0f},
        glm::vec3{ 1.0f,  1.0f,  1.0f},
        glm::vec3{-1.0f,  1.0f,  1.0f},
        glm::vec3{-1.0f, -1.0f,  1.0f},
};

static glm::vec3 face_nx[] = {
        // Negative X (NX)
        glm::vec3{-1.0f,  1.0f,  1.0f},
        glm::vec3{-1.0f,  1.0f, -1.0f},
        glm::vec3{-1.0f, -1.0f, -1.0f},
        glm::vec3{-1.0f, -1.0f, -1.0f},
        glm::vec3{-1.0f, -1.0f,  1.0f},
        glm::vec3{-1.0f,  1.0f,  1.0f},
};

static glm::vec3 face_px[] = {
        // Positive X (PX)
        glm::vec3{ 1.0f,  1.0f,  1.0f},
        glm::vec3{ 1.0f,  1.0f, -1.0f},
        glm::vec3{ 1.0f, -1.0f, -1.0f},
        glm::vec3{ 1.0f, -1.0f, -1.0f},
        glm::vec3{ 1.0f, -1.0f,  1.0f},
        glm::vec3{ 1.0f,  1.0f,  1.0f},

};

static glm::vec3 face_ny[] = {
        // Negative Y (NY)
        glm::vec3{-1.0f, -1.0f, -1.0f},
        glm::vec3{ 1.0f, -1.0f, -1.0f},
        glm::vec3{ 1.0f, -1.0f,  1.0f},
        glm::vec3{ 1.0f, -1.0f,  1.0f},
        glm::vec3{-1.0f, -1.0f,  1.0f},
        glm::vec3{-1.0f, -1.0f, -1.0f},
};

static glm::vec3 face_py[] ={
        // Positive Y (PY)
        glm::vec3{-1.0f,  1.0f, -1.0f},
        glm::vec3{ 1.0f,  1.0f, -1.0f},
        glm::vec3{ 1.0f,  1.0f,  1.0f},
        glm::vec3{ 1.0f,  1.0f,  1.0f},
        glm::vec3{-1.0f,  1.0f,  1.0f},
        glm::vec3{-1.0f,  1.0f, -1.0f}
};
// clang-format on

// clang-format off
static glm::vec3 normals_nz[] = {
        glm::vec3{0.0f,  0.0f,  -1.0f},
        glm::vec3{0.0f,  0.0f,  -1.0f},
        glm::vec3{0.0f,  0.0f,  -1.0f},
        glm::vec3{0.0f,  0.0f,  -1.0f},
        glm::vec3{0.0f,  0.0f,  -1.0f},
        glm::vec3{0.0f,  0.0f,  -1.0f},
};

static glm::vec3 normals_pz[] = {
        glm::vec3{0.0f,  0.0f,   1.0f},
        glm::vec3{0.0f,  0.0f,   1.0f},
        glm::vec3{0.0f,  0.0f,   1.0f},
        glm::vec3{0.0f,  0.0f,   1.0f},
        glm::vec3{0.0f,  0.0f,   1.0f},
        glm::vec3{0.0f,  0.0f,   1.0f},
};

static glm::vec3 normals_nx[] = {
        glm::vec3{-1.0f,  0.0f,  0.0f},
        glm::vec3{-1.0f,  0.0f,  0.0f},
        glm::vec3{-1.0f,  0.0f,  0.0f},
        glm::vec3{-1.0f,  0.0f,  0.0f},
        glm::vec3{-1.0f,  0.0f,  0.0f},
        glm::vec3{-1.0f,  0.0f,  0.0f},
};

static glm::vec3 normals_px[] = {
        glm::vec3{1.0f,  0.0f,  0.0f},
        glm::vec3{1.0f,  0.0f,  0.0f},
        glm::vec3{1.0f,  0.0f,  0.0f},
        glm::vec3{1.0f,  0.0f,  0.0f},
        glm::vec3{1.0f,  0.0f,  0.0f},
        glm::vec3{1.0f,  0.0f,  0.0f},
};

static glm::vec3 normals_ny[] = {
        glm::vec3{0.0f, -1.0f,  0.0f},
        glm::vec3{0.0f, -1.0f,  0.0f},
        glm::vec3{0.0f, -1.0f,  0.0f},
        glm::vec3{0.0f, -1.0f,  0.0f},
        glm::vec3{0.0f, -1.0f,  0.0f},
        glm::vec3{0.0f, -1.0f,  0.0f},
};

static glm::vec3 normals_py[] = {
        glm::vec3{0.0f,  1.0f,  0.0f},
        glm::vec3{0.0f,  1.0f,  0.0f},
        glm::vec3{0.0f,  1.0f,  0.0f},
        glm::vec3{0.0f,  1.0f,  0.0f},
        glm::vec3{0.0f,  1.0f,  0.0f},
        glm::vec3{0.0f,  1.0f,  0.0f}
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

        int x, y, z, vertex_count;
        size_t size;

        for (int i = 0; i < CHUNK_SIZE_CUBED; ++i) {
                convert_to_pos_in_chunk(i, &x, &y, &z);

                if (blocks[i] == Block::AIR)
                        continue;

                // Negative Z
                if (z - 1 < 0 ||
                    blocks[convert_to_block_idx(x, y, z - 1)] == Block::AIR) {
                        for (int j = 0; j < 6; ++j) {
                                current->position =
                                    face_nz[j] + glm::vec3{x, y, z};
                                current->normal = normals_nz[j];
                                current->block_type =
                                    static_cast<int>(blocks[i]);
                                current++;
                        }
                }
                // Positive Z
                if (z + 1 > 31 ||
                    blocks[convert_to_block_idx(x, y, z + 1)] == Block::AIR) {
                        for (int j = 0; j < 6; ++j) {
                                current->position =
                                    face_pz[j] + glm::vec3{x, y, z};
                                current->normal = normals_pz[j];
                                current->block_type =
                                    static_cast<int>(blocks[i]);
                                current++;
                        }
                }
                // Negative X
                if (x - 1 < 0 ||
                    blocks[convert_to_block_idx(x - 1, y, z)] == Block::AIR) {
                        for (int j = 0; j < 6; ++j) {
                                current->position =
                                    face_nx[j] + glm::vec3{x, y, z};
                                current->normal = normals_nx[j];
                                current->block_type =
                                    static_cast<int>(blocks[i]);
                                current++;
                        }
                }
                // Positive X
                if (x - 1 > 31 ||
                    blocks[convert_to_block_idx(x + 1, y, z)] == Block::AIR) {
                        for (int j = 0; j < 6; ++j) {
                                current->position =
                                    face_px[j] + glm::vec3{x, y, z};
                                current->normal = normals_px[j];
                                current->block_type =
                                    static_cast<int>(blocks[i]);
                                current++;
                        }
                }
                // Negative Y
                if (y - 1 < 0 ||
                    blocks[convert_to_block_idx(x, y - 1, z)] == Block::AIR) {
                        for (int j = 0; j < 6; ++j) {
                                current->position =
                                    face_ny[j] + glm::vec3{x, y, z};
                                current->normal = normals_ny[j];
                                current->block_type =
                                    static_cast<int>(blocks[i]);
                                current++;
                        }
                }
                // Positive Y
                if (y + 1 > 31 ||
                    blocks[convert_to_block_idx(x, y + 1, z)] == Block::AIR) {
                        for (int j = 0; j < 6; ++j) {
                                current->position =
                                    face_py[j] + glm::vec3{x, y, z};
                                current->normal = normals_py[j];
                                current->block_type =
                                    static_cast<int>(blocks[i]);
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

int Chunk::convert_to_block_idx(int x, int y, int z)
{
        return z + y * CHUNK_SIZE + x * CHUNK_SIZE_SQUARED;
}

void Chunk::convert_to_pos_in_chunk(int i, int *x, int *y, int *z)
{
        *z = i % CHUNK_SIZE;
        *y = (i % CHUNK_SIZE_SQUARED) / CHUNK_SIZE;
        *x = (i % CHUNK_SIZE_CUBED) / CHUNK_SIZE_SQUARED;
}
} // namespace kwnc
