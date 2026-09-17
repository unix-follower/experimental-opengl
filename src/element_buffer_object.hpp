#pragma once

#include <glad/glad.h>
#include <vector>

class EBO {
  public:
    GLuint id;
    EBO(const GLuint* indices, GLsizeiptr size);
    EBO(std::vector<GLuint>& indices);

    void bind() const;

    static void unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void deleteEBO() const;
};
