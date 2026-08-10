#include "element_buffer_object.hpp"

EBO::EBO(GLuint const* indices, GLsizeiptr size)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void EBO::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void EBO::deleteEBO() const
{
    glDeleteBuffers(1, &id);
}
