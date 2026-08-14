#include "basic_shapes.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

#include "element_buffer_object.hpp"
#include "shader.hpp"
#include "vertex_array_object.hpp"
#include "vertex_buffer_object.hpp"

const GLfloat vertices[] = {
    // clang-format off
	//               COORDINATES                  /     COLORS           //
	-0.5F, -0.5F * float(sqrt(3)) * 1 / 3, 0.0F,     0.8F, 0.3F,  0.02F, // Lower left corner
	 0.5F, -0.5F * float(sqrt(3)) * 1 / 3, 0.0F,     0.8F, 0.3F,  0.02F, // Lower right corner
	 0.0F,  0.5F * float(sqrt(3)) * 2 / 3, 0.0F,     1.0F, 0.6F,  0.32F, // Upper corner
	-0.25F, 0.5F * float(sqrt(3)) * 1 / 6, 0.0F,     0.9F, 0.45F, 0.17F, // Inner left
	 0.25F, 0.5F * float(sqrt(3)) * 1 / 6, 0.0F,     0.9F, 0.45F, 0.17F, // Inner right
	 0.0F, -0.5F * float(sqrt(3)) * 1 / 3, 0.0F,     0.8F, 0.3F,  0.02F  // Inner down
    // clang-format on
};

const GLuint indices[] = {
    // clang-format off
	0, 3, 5, // Lower left triangle
	3, 2, 4, // Lower right triangle
	5, 4, 1 // Upper triangle
    // clang-format on
};

int basic_shapes::showExampleWithShaders()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Shaders demo", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, 800, 800);

    Shader shaderProgram("shaders/shaders_demo.vert", "shaders/shaders_demo.frag");

    VAO vao;
    vao.bind();

    VBO vbo(vertices, sizeof(vertices));
    EBO ebo(indices, sizeof(indices));

    VAO::linkAttrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    VAO::linkAttrib(vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO::unbind();
    VBO::unbind();
    EBO::unbind();

    GLuint uniID = glGetUniformLocation(shaderProgram.id, "scale");

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.07F, 0.13F, 0.17F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.activate();
        glUniform1f(static_cast<GLint>(uniID), 0.5F);
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
