#ifndef MAP_HPP
#define MAP_HPP

#include "chunk.hpp"
#include "FastNoiseLite.h"
#include "shader.hpp"
#include <glm/glm.hpp>
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
        // ~Map();
        std::unordered_map<std::tuple<int, int, int>, std::shared_ptr<Chunk>,
                           chunk_coordinate_hash>
            chunks;

        void setup(const glm::vec3 &camera_position);
        void update(const glm::vec3 &camera_position);
        void render(const Shader &shader);

private:
        int last_camera_chunk_x, last_camera_chunk_y, last_camera_chunk_z;
        FastNoiseLite noise;
        void new_chunks_x(int dx, int camera_chunk_x, int camera_chunk_y,
                          int camera_chunk_z);
        void new_chunks_y(int dy, int camera_chunk_x, int camera_chunk_y,
                          int camera_chunk_z);
        void new_chunks_z(int dz, int camera_chunk_x, int camera_chunk_y,
                          int camera_chunk_z);
};
} // namespace kwnc

#endif // !MAP_HPP
