#include "basic_shapes.hpp"
#include "element_buffer_object.hpp"
#include "shader.hpp"
#include "vertex_array_object.hpp"
#include "vertex_buffer_object.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

namespace {
const GLfloat vertices[] = {
    // clang-format off
	-0.5F, -0.5F * float(sqrt(3)) / 3, 0.0F, // Lower left corner
	0.5F, -0.5F * float(sqrt(3)) / 3, 0.0F, // Lower right corner
	0.0f, 0.5F * float(sqrt(3)) * 2 / 3, 0.0F, // Upper corner
	-0.5F / 2, 0.5F * float(sqrt(3)) / 6, 0.0F, // Inner left
	0.5F / 2, 0.5F * float(sqrt(3)) / 6, 0.0F, // Inner right
	0.0F, -0.5F * float(sqrt(3)) / 3, 0.0F // Inner down
    // clang-format on
};

const GLuint indices[] = {
    // clang-format off
	0, 3, 5, // Lower left triangle
	3, 2, 4, // Lower right triangle
	5, 4, 1 // Upper triangle
    // clang-format on
};
} // namespace

int basic_shapes::showExampleWithProjectLayout()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Project Layout", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glViewport(0, 0, 800, 800);

    Shader shaderProgram("shaders/default.vert", "shaders/default.frag");

    VAO vao;
    vao.bind();

    VBO vbo(vertices, sizeof(vertices));
    EBO ebo(indices, sizeof(indices));

    vao.linkVBO(vbo, 0);
    vao.unbind();
    vbo.unbind();
    ebo.unbind();

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.activate();
        vao.bind();
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vao.deleteVAO();
    vbo.deleteVBO();
    ebo.deleteEBO();
    shaderProgram.deleteShader();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
