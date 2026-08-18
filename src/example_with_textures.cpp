#include "basic_shapes.hpp"

#include <cmath>
#include <filesystem>
#include <iostream>

#include "element_buffer_object.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex_array_object.hpp"
#include "vertex_buffer_object.hpp"

namespace fs = std::filesystem;

GLfloat const vertices[] = {
    // clang-format off
    //   COORDINATES     /        COLORS      /   TexCoord  //
	-0.5F, -0.5F, 0.0F,     1.0F, 0.0F, 0.0F,	0.0F, 0.0F, // Lower left corner
	-0.5F,  0.5F, 0.0F,     0.0F, 1.0F, 0.0F,	0.0F, 1.0F, // Upper left corner
	 0.5F,  0.5F, 0.0F,     0.0F, 0.0F, 1.0F,	1.0F, 1.0F, // Upper right corner
	 0.5F, -0.5F, 0.0F,     1.0F, 1.0F, 1.0F,	1.0F, 0.0F  // Lower right corner
    // clang-format on
};

GLuint const indices[] = {
    // clang-format off
	0, 2, 1, // Upper triangle
	0, 3, 2 // Lower triangle
    // clang-format on
};

namespace basic_shapes {
int showExampleWithTextures()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Texture example", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, 800, 800);

    Shader shaderProgram("shaders/example_with_textures.vert",
                         "shaders/example_with_textures.frag");

    VAO vao;
    vao.bind();

    VBO vbo(vertices, sizeof(vertices));
    EBO ebo(indices, sizeof(indices));

    VAO::linkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO::linkAttrib(
        vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), static_cast<char*>(nullptr) + (3 * sizeof(float)));
    VAO::linkAttrib(
        vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), static_cast<char*>(nullptr) + (6 * sizeof(float)));
    VAO::unbind();
    VBO::unbind();
    EBO::unbind();

    GLuint uniID = glGetUniformLocation(shaderProgram.id, "scale");

    Texture popCat("resources/pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture::texUnit(shaderProgram, "tex0", 0);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.07F, 0.13F, 0.17F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.activate();
        glUniform1f(static_cast<GLint>(uniID), 0.5F);
        popCat.bind();
        vao.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vao.deleteVAO();
    vbo.deleteVBO();
    ebo.deleteEBO();
    popCat.deleteTexture();
    shaderProgram.deleteShader();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
} // namespace basic_shapes
