#pragma once

#include "vertex_buffer_object.hpp"
#include <glad/glad.h>

class VAO {
  public:
    GLuint id;
    VAO();

    static void linkVBO(VBO const& vbo, GLuint layout)
    {
        vbo.bind();
        glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(layout);
        VBO::unbind();
    }

    static void linkAttrib(VBO const& vbo,
                           GLuint layout,
                           GLuint numComponents,
                           GLenum type,
                           GLsizeiptr stride,
                           void* offset)
    {
        vbo.bind();
        glVertexAttribPointer(layout,
                              static_cast<GLint>(numComponents),
                              type,
                              GL_FALSE,
                              static_cast<GLint>(stride),
                              offset);
        glEnableVertexAttribArray(layout);
        VBO::unbind();
    }

    void bind() const;

    static void unbind()
    {
        glBindVertexArray(0);
    }

    void deleteVAO() const;
};
