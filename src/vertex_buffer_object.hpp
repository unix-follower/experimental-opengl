#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texUV;
};

class VBO {
  public:
    GLuint id;
    VBO(const GLfloat* vertices, GLsizeiptr size);
    explicit VBO(std::vector<Vertex>& vertices);

    void bind() const;

    static void unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void deleteVBO() const;
};
