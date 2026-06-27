#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <cstdint>
#include <glm/glm.hpp>

namespace kwnc
{
enum class Block : uint8_t { DIRT, STONE, AIR };

// TODO: Can we optimize this even more?
struct Block_Vertex {
  glm::vec3 normal;               // 12 Bytes
  glm::vec2 uv;                   // 8 Bytes
  unsigned int packed_coord_and_type; // 4 bytes
};
}; // namespace kwnc

#endif // !BLOCK_HPP
