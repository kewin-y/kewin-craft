#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <glm/glm.hpp>
#include <cstdint>

namespace kwnc
{
enum class Block : uint8_t { DIRT, STONE, AIR };

struct Block_Vertex {
        glm::vec3 normal;       // 12 Bytes
        glm::vec2 uv;           // 8 Bytes
        unsigned int packed_coord_type; // 4 bytes
};
}; // namespace kwnc

#endif // !BLOCK_HPP
