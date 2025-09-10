#ifndef BLOCK_VERTEX_ARRAY_HPP
#define BLOCK_VERTEX_ARRAY_HPP

#include "block.hpp"
#include <cstddef>

namespace kwnc
{
class Block_Vertex_Array
{
public:
        Block_Vertex_Array() = default;
        ~Block_Vertex_Array();

        void init();
        void draw();
        void buffer_data(size_t size, int vertex_count,
                         Block_Vertex *vertex_data);

        bool initialized = false;

private:
        unsigned int id{0};
        unsigned int vbo{0};
        int vertex_count;
};
} // namespace kwnc

#endif // !BLOCK_VERTEX_ARRAY_HPP
