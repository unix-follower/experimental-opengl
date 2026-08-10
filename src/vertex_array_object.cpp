#include "vertex_array_object.hpp"

VAO::VAO()
{
    glGenVertexArrays(1, &id);
}

void VAO::bind() const
{
    glBindVertexArray(id);
}

void VAO::deleteVAO() const
{
    glDeleteVertexArrays(1, &id);
}
