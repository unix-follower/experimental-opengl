#include "texture.hpp"

Texture::Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType)
{
    type = texType;

    int imageWidth;
    int imageHeight;
    int numColorChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(image, &imageWidth, &imageHeight, &numColorChannels, 0);
    if (!bytes) {
        std::cout << "Failed to load texture: " << image << "\n";
    }

    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0 + slot);
    unit = slot;
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
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(type, id);
}

void Texture::unbind() const
{
    glBindTexture(type, 0);
}

TypeStringBasedTexture::TypeStringBasedTexture(
    const char* image, const char* texType, GLuint slot, GLenum format, GLenum pixelType)
{
    type = texType;

    int widthImg;
    int heightImg;
    int numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0 + slot);
    unit = slot;
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void TypeStringBasedTexture::bind() const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

void TypeStringBasedTexture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
