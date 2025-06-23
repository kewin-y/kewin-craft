#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "block.hpp"
#include "block_vertex_array.hpp"
#include <glm/glm.hpp>

namespace kwnc
{
class Chunk
{
public:
        static constexpr int CHUNK_SIZE = 32;
        static constexpr int CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
        static constexpr int CHUNK_SIZE_CUBED =
            CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

        glm::vec3 position;
        Block_Vertex_Array vertex_array;

        Chunk(int x, int y, int z);
        ~Chunk();

        void generate_mesh();

private:
        Block *blocks;

        int chunk_pos_to_idx(int x, int y, int z);
        void idx_to_chunk_pos(int i, int *x, int *y, int *z);
};
} // namespace kwnc

#endif // !CHUNK_HPP
