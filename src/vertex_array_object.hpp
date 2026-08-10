#pragma once

#include "vertex_buffer_object.hpp"
#include <glad/glad.h>

class VAO {
  public:
    GLuint id;
    VAO();

    void linkVBO(VBO& vbo, GLuint layout) const;
    void bind() const;
    void unbind() const;
    void deleteVAO() const;
};
