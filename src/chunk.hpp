#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "block.hpp"
#include "block_vertex_array.hpp"

namespace kwnc
{
class Chunk
{
public:
        static constexpr int CHUNK_SIZE = 32;
        static constexpr int CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
        static constexpr int CHUNK_SIZE_CUBED =
            CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

        Block_Vertex_Array vertex_array;

        Chunk(int x, int y, int z);
        ~Chunk();

        void generate_mesh();

private:
        int chunk_x; // TODO: Do something with these
        int chunk_y;
        int chunk_z;
        Block *blocks;
};
} // namespace kwnc

#endif // !CHUNK_HPP
