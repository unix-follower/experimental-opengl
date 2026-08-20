#include "basic_shapes.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "element_buffer_object.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex_array_object.hpp"
#include "vertex_buffer_object.hpp"

const unsigned int width = 800;
const unsigned int height = 800;

const GLfloat vertices[] = {
    // clang-format off
    //   COORDINATES     /        COLORS      /   TexCoord  //
	-0.5F, 0.0F,  0.5F,     0.83F, 0.70F, 0.44F,	0.0F, 0.0F,
	-0.5F, 0.0F, -0.5F,     0.83F, 0.70F, 0.44F,	5.0F, 0.0F,
	 0.5F, 0.0F, -0.5F,     0.83F, 0.70F, 0.44F,	0.0F, 0.0F,
	 0.5F, 0.0F,  0.5F,     0.83F, 0.70F, 0.44F,	5.0F, 0.0F,
	 0.0F, 0.8F,  0.0F,     0.92F, 0.86F, 0.76F,	2.5F, 5.0F
    // clang-format on
};

const GLuint indices[] = {
    // clang-format off
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
    // clang-format on
};

namespace {
void render(GLuint uniID,
            const VAO& vao,
            GLFWwindow* window,
            const Shader& shaderProgram,
            const Texture& brickTexture,
            float& rotation,
            double& prevTime)
{
    glClearColor(0.07F, 0.13F, 0.17F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderProgram.activate();

    if (double crntTime = glfwGetTime(); crntTime - prevTime >= 1.0 / 60) {
        rotation += 0.5F;
        prevTime = crntTime;
    }

    auto model = glm::mat4(1.0F);
    auto view = glm::mat4(1.0F);
    auto proj = glm::mat4(1.0F);

    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0F, 1.0F, 0.0F));
    view = glm::translate(view, glm::vec3(0.0F, -0.5F, -2.0F));
    proj = glm::perspective(glm::radians(45.0F), (float)width / height, 0.1F, 100.0F);

    int modelLoc = glGetUniformLocation(shaderProgram.id, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    int viewLoc = glGetUniformLocation(shaderProgram.id, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    int projLoc = glGetUniformLocation(shaderProgram.id, "proj");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

    glUniform1f(static_cast<GLint>(uniID), 0.5F);
    brickTexture.bind();
    vao.bind();
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
    glfwPollEvents();
}
} // namespace

namespace basic_shapes {
int showExampleWith3D()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "3D example", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, width, height);

    Shader shaderProgram("shaders/example_with_3d.vert", "shaders/example_with_3d.frag");

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

    Texture brickTex("resources/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture::texUnit(shaderProgram, "tex0", 0);

    float rotation = 0.0F;
    double prevTime = glfwGetTime();

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        render(uniID, vao, window, shaderProgram, brickTex, rotation, prevTime);
    }

    vao.deleteVAO();
    vbo.deleteVBO();
    ebo.deleteEBO();
    brickTex.deleteTexture();
    shaderProgram.deleteShader();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
} // namespace basic_shapes
