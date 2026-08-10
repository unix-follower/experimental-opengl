#pragma once

#include <glad/glad.h>

class VBO {
  public:
    GLuint id;
    VBO(GLfloat const* vertices, GLsizeiptr size);

    void bind() const;
    void unbind() const;
    void deleteVBO() const;
};
