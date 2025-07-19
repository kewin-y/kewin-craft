#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <sys/types.h>

namespace kwnc
{
enum class Block : uint8_t { DIRT, STONE, AIR };

struct Block_Vertex {
        glm::vec3 normal;       // 12 Bytes
        glm::vec2 uv;           // 8 Bytes
        uint packed_coord_type; // 4 bytes
};
}; // namespace kwnc

#endif // !BLOCK_HPP
