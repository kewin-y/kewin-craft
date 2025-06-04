#include "block_vertex_array.hpp"
#include "block.hpp"
#include "glad/gl.h"

namespace kwnc
{
Block_Vertex_Array::Block_Vertex_Array()
{
        vertex_count = 0;

        // Setup VAO
        glGenVertexArrays(1, &id);
        glGenBuffers(1, &vbo);
        glBindVertexArray(id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // a_position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                              6 * sizeof(float) + sizeof(int), nullptr);
        glEnableVertexAttribArray(0);

        // a_normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                              6 * sizeof(float) + sizeof(int),
                              (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // a_block_type
        glVertexAttribPointer(2, 1, GL_INT, GL_FALSE,
                              6 * sizeof(float) + sizeof(int),
                              (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Block_Vertex_Array::~Block_Vertex_Array()
{
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &id);
}

void Block_Vertex_Array::buffer_data(size_t size, int vertex_count, Block_Vertex *vertex_data)
{
        this->vertex_count = vertex_count;

        glBindVertexArray(id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, size, vertex_data, GL_STATIC_DRAW);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Block_Vertex_Array::draw()
{
        glBindVertexArray(id);
        glDrawArrays(GL_TRIANGLES, 0, vertex_count);
        glBindVertexArray(0);
}
} // namespace kwnc
