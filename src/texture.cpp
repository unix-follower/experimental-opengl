#include "texture.hpp"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
    type = texType;

    int imageWidth;
    int imageHeight;
    int numColorChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(image, &imageWidth, &imageHeight, &numColorChannels, 0);

    glGenTextures(1, &id);
    glActiveTexture(slot);
    glBindTexture(texType, id);

    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(texType, 0, GL_RGBA, imageWidth, imageHeight, 0, format, pixelType, bytes);
    glGenerateMipmap(texType);

    stbi_image_free(bytes);

    glBindTexture(texType, 0);
}

void Texture::bind() const
{
    glBindTexture(type, id);
}

void Texture::unbind() const
{
    glBindTexture(type, 0);
}

void Texture::deleteTexture() const
{
    glDeleteTextures(1, &id);
}
