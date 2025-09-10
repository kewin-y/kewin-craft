#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "FastNoiseLite.h"
#include "block_vertex_array.hpp"
#include <glm/glm.hpp>
#include <cstdint>
#include <mutex>
#include <atomic>

namespace kwnc
{
class Chunk
{
public:
        static constexpr int CHUNK_SIZE = 32;
        static constexpr int CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
        static constexpr int CHUNK_SIZE_CUBED =
            CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
        static constexpr int TERRAIN_CONSTANT = 64;

        int chunk_x, chunk_y, chunk_z;
        Block_Vertex_Array vertex_array;

        std::atomic<bool> dirty{true};

        Chunk(int x, int y, int z);
        ~Chunk();

        void generate_terrain(const FastNoiseLite &noise);
        // void set_voxel(int x, int y, int z, Block type);
        // void reset();
        void generate_mesh();

private:
        Block *blocks;
        std::mutex mtx;

        static int convert_to_block_idx(int x, int y, int z);
        static void convert_to_pos_in_chunk(int i, int *x, int *y, int *z);
        static glm::vec2 get_uv(uint8_t type, const glm::vec2 &local_uv);
};
} // namespace kwnc

#endif // !CHUNK_HPP
