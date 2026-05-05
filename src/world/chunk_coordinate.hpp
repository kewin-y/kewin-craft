#ifndef CHUNK_COORDINATE_HPP
#define CHUNK_COORDINATE_HPP

#include <cstdlib>
#include <functional>

// https://stackoverflow.com/questions/35985960/c-why-is-boosthash-combine-the-best-way-to-combine-hash-values
template <typename T>
static inline void hash_combine(std::size_t &seed, const T &v)
{
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace kwnc
{
class Chunk_Coordinate
{
public:
  const int x;
  const int y;
  const int z;

  Chunk_Coordinate(int x, int y, int z) : x{x}, y{y}, z{z} {}

  friend bool operator==(const Chunk_Coordinate &lhs,
                         const Chunk_Coordinate &rhs)
  {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
  }
};

struct Chunk_Coordinate_Hasher {
  size_t operator()(const Chunk_Coordinate &coord) const
  {
    size_t seed = 0;
    hash_combine(seed, coord.x);
    hash_combine(seed, coord.y);
    hash_combine(seed, coord.z);
    return seed;
  }
};
} // namespace kwnc

#endif
