#include "chunk.hpp"
#include "block.hpp"
#include "block_vertex_array.hpp"
#include "glad/gl.h"
#include <glm/glm.hpp>
#include <iostream>

// I promise I'll make this code better

// clang-format off
static glm::vec3 positions[] = {
    glm::vec3{-1.0f, -1.0f, -1.0f},
    glm::vec3{ 1.0f, -1.0f, -1.0f},
    glm::vec3{ 1.0f,  1.0f, -1.0f},
    glm::vec3{ 1.0f,  1.0f, -1.0f},
    glm::vec3{-1.0f,  1.0f, -1.0f},
    glm::vec3{-1.0f, -1.0f, -1.0f},

    glm::vec3{-1.0f, -1.0f,  1.0f},
    glm::vec3{ 1.0f, -1.0f,  1.0f},
    glm::vec3{ 1.0f,  1.0f,  1.0f},
    glm::vec3{ 1.0f,  1.0f,  1.0f},
    glm::vec3{-1.0f,  1.0f,  1.0f},
    glm::vec3{-1.0f, -1.0f,  1.0f},

    glm::vec3{-1.0f,  1.0f,  1.0f},
    glm::vec3{-1.0f,  1.0f, -1.0f},
    glm::vec3{-1.0f, -1.0f, -1.0f},
    glm::vec3{-1.0f, -1.0f, -1.0f},
    glm::vec3{-1.0f, -1.0f,  1.0f},
    glm::vec3{-1.0f,  1.0f,  1.0f},

    glm::vec3{ 1.0f,  1.0f,  1.0f},
    glm::vec3{ 1.0f,  1.0f, -1.0f},
    glm::vec3{ 1.0f, -1.0f, -1.0f},
    glm::vec3{ 1.0f, -1.0f, -1.0f},
    glm::vec3{ 1.0f, -1.0f,  1.0f},
    glm::vec3{ 1.0f,  1.0f,  1.0f},

    glm::vec3{-1.0f, -1.0f, -1.0f},
    glm::vec3{ 1.0f, -1.0f, -1.0f},
    glm::vec3{ 1.0f, -1.0f,  1.0f},
    glm::vec3{ 1.0f, -1.0f,  1.0f},
    glm::vec3{-1.0f, -1.0f,  1.0f},
    glm::vec3{-1.0f, -1.0f, -1.0f},

    glm::vec3{-1.0f,  1.0f, -1.0f},
    glm::vec3{ 1.0f,  1.0f, -1.0f},
    glm::vec3{ 1.0f,  1.0f,  1.0f},
    glm::vec3{ 1.0f,  1.0f,  1.0f},
    glm::vec3{-1.0f,  1.0f,  1.0f},
    glm::vec3{-1.0f,  1.0f, -1.0f}
};

static glm::vec3 normals[] = {
    glm::vec3{0.0f,  0.0f,  -1.0f},
    glm::vec3{0.0f,  0.0f,  -1.0f},
    glm::vec3{0.0f,  0.0f,  -1.0f},
    glm::vec3{0.0f,  0.0f,  -1.0f},
    glm::vec3{0.0f,  0.0f,  -1.0f},
    glm::vec3{0.0f,  0.0f,  -1.0f},

    glm::vec3{0.0f,  0.0f,   1.0f},
    glm::vec3{0.0f,  0.0f,   1.0f},
    glm::vec3{0.0f,  0.0f,   1.0f},
    glm::vec3{0.0f,  0.0f,   1.0f},
    glm::vec3{0.0f,  0.0f,   1.0f},
    glm::vec3{0.0f,  0.0f,   1.0f},

    glm::vec3{-1.0f,  0.0f,  0.0f},
    glm::vec3{-1.0f,  0.0f,  0.0f},
    glm::vec3{-1.0f,  0.0f,  0.0f},
    glm::vec3{-1.0f,  0.0f,  0.0f},
    glm::vec3{-1.0f,  0.0f,  0.0f},
    glm::vec3{-1.0f,  0.0f,  0.0f},

    glm::vec3{1.0f,  0.0f,  0.0f},
    glm::vec3{1.0f,  0.0f,  0.0f},
    glm::vec3{1.0f,  0.0f,  0.0f},
    glm::vec3{1.0f,  0.0f,  0.0f},
    glm::vec3{1.0f,  0.0f,  0.0f},
    glm::vec3{1.0f,  0.0f,  0.0f},

    glm::vec3{0.0f, -1.0f,  0.0f},
    glm::vec3{0.0f, -1.0f,  0.0f},
    glm::vec3{0.0f, -1.0f,  0.0f},
    glm::vec3{0.0f, -1.0f,  0.0f},
    glm::vec3{0.0f, -1.0f,  0.0f},
    glm::vec3{0.0f, -1.0f,  0.0f},

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
Chunk::Chunk(int x, int y, int z) : vertex_array{}
{
        chunk_x = x;
        chunk_y = y;
        chunk_z = z;

        // TODO: Wrap this in a try-catch
        blocks = new Block[CHUNK_SIZE_CUBED];

        for (int i = 0; i < CHUNK_SIZE_CUBED; i++) {
                blocks[i] = Block::STONE;
        }
}

Chunk::~Chunk()
{
        delete[] blocks;
}

void Chunk::generate_mesh()
{
        // NOTE: 0 0 0 is the top-left block on the most bottom layer
        Block_Vertex *vertex_data = new Block_Vertex[36 * CHUNK_SIZE_CUBED];
        Block_Vertex *current = vertex_data;

        int x, y, z, vertex_count;
        size_t size;

        for (int i = 0; i < CHUNK_SIZE_CUBED; i++) {
                x = i % CHUNK_SIZE;
                y = (i % CHUNK_SIZE_SQUARED) / CHUNK_SIZE;
                z = (i % CHUNK_SIZE_CUBED) / CHUNK_SIZE_SQUARED;

                for (int j = 0; j < 36; j++) {
                        current->position = positions[j] + glm::vec3{x, y, z};
                        current->normal = normals[j];
                        current->block_type = (int)blocks[i];
                        current++;
                }
        }

        vertex_count = current - vertex_data;
        size = vertex_count * sizeof(Block_Vertex);
        vertex_array.buffer_data(size, vertex_count, vertex_data);
        std::cout << "There are: " << current - vertex_data << " vertices\n";

        delete[] vertex_data;
}
} // namespace kwnc
