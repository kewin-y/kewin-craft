#ifndef BLOCK_VERTEX_ARRAY_HPP
#define BLOCK_VERTEX_ARRAY_HPP

#include "block.hpp"

namespace kwnc
{
class Block_Vertex_Array
{
public:
        Block_Vertex_Array();
        ~Block_Vertex_Array();

        void draw();
        void buffer_data(size_t size, int vertex_count,
                         Block_Vertex *vertex_data);

private:
        unsigned int id;
        unsigned int vbo;
        int vertex_count;
};
} // namespace kwnc

#endif // !BLOCK_VERTEX_ARRAY_HPP
