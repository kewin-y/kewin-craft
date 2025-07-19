#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "block.hpp"
#include "block_vertex_array.hpp"
#include <cstdint>
#include <glm/glm.hpp>
#include <sys/types.h>

namespace kwnc
{
class Chunk
{
public:
        static constexpr int CHUNK_SIZE = 32;
        static constexpr int CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
        static constexpr int CHUNK_SIZE_CUBED =
            CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

        int chunk_x, chunk_y, chunk_z;
        Block_Vertex_Array vertex_array;

        Chunk(int x, int y, int z);
        ~Chunk();

        bool is_empty();
        bool is_dirty();

        void add_voxel(int x, int y, int z, Block type);
        void fill();
        void reset();
        void generate_mesh();

private:
        Block *blocks = nullptr;
        bool dirty = false;

        static int convert_to_block_idx(int x, int y, int z);
        static void convert_to_pos_in_chunk(int i, int *x, int *y, int *z);
        static glm::vec2 get_uv(uint8_t type, const glm::vec2 &local_uv);
};
} // namespace kwnc

#endif // !CHUNK_HPP
