#pragma once

#include <string>

#include "camera.hpp"
#include "element_buffer_object.hpp"
#include "texture.hpp"
#include "vertex_array_object.hpp"
#include <memory>
#include <vector>

class Mesh {
    std::vector<Vertex> vertices_;
    std::vector<GLuint> indices_;
    std::vector<std::unique_ptr<BaseTexture>> textures_;
    VAO vao_;

  public:
    Mesh(std::vector<Vertex>& vertices,
         std::vector<GLuint>& indices,
         std::vector<std::unique_ptr<BaseTexture>>& textures);

    void draw(const Shader& shader, Camera& camera);
};
