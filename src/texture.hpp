#pragma once

#include <glad/glad.h>
#include <stb_image.h>

#include "shader.hpp"

class Texture {
  public:
    GLuint id;
    GLenum type;
    Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

    static void texUnit(const Shader& shader, const char* uniform, GLuint unit)
    {
        GLuint texUni = glGetUniformLocation(shader.id, uniform);
        shader.activate();
        glUniform1i(static_cast<GLint>(texUni), static_cast<GLint>(unit));
    }

    void bind() const;
    void unbind() const;
    void deleteTexture() const;
};
