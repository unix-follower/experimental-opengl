#pragma once

#include <glad/glad.h>

class EBO {
  public:
    GLuint id;
    EBO(GLuint const* indices, GLsizeiptr size);

    void bind() const;

    static void unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void deleteEBO() const;
};
