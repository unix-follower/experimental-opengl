#pragma once

#include <glad/glad.h>
#include <stb_image.h>

#include "shader.hpp"

class BaseTexture {
  public:
    GLuint id;
    GLuint unit;

    virtual ~BaseTexture() = default;

    static void texUnit(const Shader& shader, const char* uniform, GLuint unit)
    {
        GLuint texUni = glGetUniformLocation(shader.id, uniform);
        shader.activate();
        glUniform1i(static_cast<GLint>(texUni), static_cast<GLint>(unit));
    }

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void deleteTexture() const
    {
        glDeleteTextures(1, &id);
    }
};

class Texture : public BaseTexture {
  public:
    GLenum type;
    Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

    void bind() const override;
    void unbind() const override;
};

class TypeStringBasedTexture : public BaseTexture {
  public:
    const char* type;
    TypeStringBasedTexture(
        const char* image, const char* texType, GLuint slot, GLenum format, GLenum pixelType);

    void bind() const override;
    void unbind() const override;
};
