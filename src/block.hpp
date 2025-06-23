#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <glm/vec3.hpp>
#include <sys/types.h>

namespace kwnc
{
enum class Block : u_int16_t { AIR, DIRT, STONE };

struct Block_Vertex {
        glm::vec3 position; // Represents chunk coordinates
        glm::vec3 normal;
        int block_type;
};
}; // namespace kwnc

#endif // !BLOCK_HPP
