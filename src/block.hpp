#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <glm/vec3.hpp>

namespace kwnc
{
enum class Block : int { AIR, DIRT, STONE };

struct Block_Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        int block_type;
};
}; // namespace kwnc

#endif // !BLOCK_HPP
