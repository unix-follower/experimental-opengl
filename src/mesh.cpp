#include "mesh.hpp"

Mesh::Mesh(std::vector<Vertex>& vertices,
           std::vector<GLuint>& indices,
           std::vector<std::unique_ptr<BaseTexture>>& textures)
{
    vertices_ = vertices;
    indices_ = indices;
    textures_ = std::move(textures);

    vao_.bind();
    VBO vbo(vertices);
    EBO ebo(indices);
    VAO::linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), static_cast<char*>(nullptr) + 0);
    VAO::linkAttrib(
        vbo, 1, 3, GL_FLOAT, sizeof(Vertex), static_cast<char*>(nullptr) + (3 * sizeof(float)));
    VAO::linkAttrib(
        vbo, 2, 3, GL_FLOAT, sizeof(Vertex), static_cast<char*>(nullptr) + (6 * sizeof(float)));
    VAO::linkAttrib(
        vbo, 3, 2, GL_FLOAT, sizeof(Vertex), static_cast<char*>(nullptr) + (9 * sizeof(float)));

    VAO::unbind();
    VBO::unbind();
    EBO::unbind();
}

void Mesh::draw(const Shader& shader, Camera& camera)
{
    shader.activate();
    vao_.bind();

    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    for (unsigned int i = 0; i < textures_.size(); i++) {
        if (const auto* texture = dynamic_cast<TypeStringBasedTexture*>(textures_[i].get());
            texture != nullptr) {
            std::string num;
            std::string type = texture->type;
            if (type == "diffuse") {
                num = std::to_string(numDiffuse++);
            }
            else if (type == "specular") {
                num = std::to_string(numSpecular++);
            }
            Texture::texUnit(shader, (type + num).c_str(), i);
        }
        textures_[i]->bind();
    }
    glUniform3f(glGetUniformLocation(shader.id, "cameraPosition"),
                camera.position.x,
                camera.position.y,
                camera.position.z);
    camera.matrix(shader, "cameraMatrix");

    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
}
