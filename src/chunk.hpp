#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "FastNoiseLite.h"
#include "block.hpp"
#include "block_vertex_array.hpp"
#include <atomic>
#include <cstdint>
#include <glm/glm.hpp>
#include <mutex>
#include <vector>

namespace kwnc
{

enum class Chunk_State : uint8_t {
        DIRTY,
        READY,
        DRAWN,
};

class Chunk
{
public:
        static constexpr int CHUNK_SIZE = 32;
        static constexpr int CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
        static constexpr int CHUNK_SIZE_CUBED =
            CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
        static constexpr int TERRAIN_CONSTANT = 64;

        int chunk_x, chunk_y, chunk_z;

        Chunk_State state = Chunk_State::DIRTY;

        Chunk(int x, int y, int z);
        ~Chunk();

        void generate_terrain(const FastNoiseLite &noise);
        void fill();
        // void set_voxel(int x, int y, int z, Block type);
        // void reset();
        void generate_mesh();
        void draw();

private:
        Block *blocks;
        std::vector<Block_Vertex> vertex_data;
        Block_Vertex_Array vertex_array;

        static int convert_to_block_idx(int x, int y, int z);
        static void convert_to_pos_in_chunk(int i, int *x, int *y, int *z);
        static glm::vec2 get_uv(uint8_t type, const glm::vec2 &local_uv);
};
} // namespace kwnc

#endif // !CHUNK_HPP
