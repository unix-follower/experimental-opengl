#pragma once

#include <glad/glad.h>

class VBO {
  public:
    GLuint id;
    VBO(const GLfloat* vertices, GLsizeiptr size);

    void bind() const;

    static void unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void deleteVBO() const;
};
