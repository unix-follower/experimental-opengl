#pragma once

#include <glad/glad.h>

class EBO {
  public:
    GLuint id;
    EBO(GLuint const* indices, GLsizeiptr size);

    void bind() const;
    void unbind() const;
    void deleteEBO() const;
};
