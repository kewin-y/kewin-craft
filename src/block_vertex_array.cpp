#include "block_vertex_array.hpp"
#include "block.hpp"
#include "glad/gl.h"
#include <cstddef>
#include <iostream>

namespace kwnc
{
void Block_Vertex_Array::init()
{
        vertex_count = 0;

        // Setup VAO
        glGenVertexArrays(1, &id);
        glGenBuffers(1, &vbo);
        glBindVertexArray(id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        size_t stride = 5 * sizeof(float) + sizeof(unsigned int);

        // a_normal
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
        glEnableVertexAttribArray(0);

        // a_uv
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride,
                              (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, stride,
                               (void *)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        initialized = true;
}

Block_Vertex_Array::~Block_Vertex_Array()
{
        if (vbo != 0)
                glDeleteBuffers(1, &vbo);
        if (id != 0)
                glDeleteVertexArrays(1, &id);
}

void Block_Vertex_Array::buffer_data(size_t size, int vertex_count,
                                     Block_Vertex *vertex_data)
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
