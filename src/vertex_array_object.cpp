#include "vertex_array_object.hpp"

VAO::VAO()
{
    glGenVertexArrays(1, &id);
}

void VAO::linkAttrib(VBO const& vbo,
                     GLuint layout,
                     GLuint numComponents,
                     GLenum type,
                     GLsizeiptr stride,
                     void* offset) const
{
    vbo.bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    VBO::unbind();
}

void VAO::bind() const
{
    glBindVertexArray(id);
}

void VAO::deleteVAO() const
{
    glDeleteVertexArrays(1, &id);
}
