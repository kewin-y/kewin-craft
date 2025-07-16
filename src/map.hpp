#ifndef MAP_HPP
#define MAP_HPP

#include "shader.hpp"
#include "chunk.hpp"
#include <glm/vec3.hpp>
#include <memory>
#include <tuple>
#include <unordered_map>

namespace kwnc
{

struct chunk_coordinate_hash {
        size_t operator()(const std::tuple<int, int, int> &coords) const;
};
class Map
{
public:
        Map();
        ~Map();
        std::unordered_map<std::tuple<int, int, int>, std::shared_ptr<Chunk>,
                           chunk_coordinate_hash>
            chunks;

        void setup(const glm::vec3 camera_position);
        void render(const Shader &shader);

private:
};
} // namespace kwnc

#endif // !MAP_HPP
