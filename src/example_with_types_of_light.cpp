#include "basic_shapes.hpp"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "camera.hpp"
#include "element_buffer_object.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex_array_object.hpp"
#include "vertex_buffer_object.hpp"

namespace {
const unsigned int width = 800;
const unsigned int height = 800;

constexpr GLfloat vertices[] = {
    // clang-format off
    //   COORDINATES     /        COLORS        /    TexCoord    /       NORMALS     //
	-1.0F, 0.0F,  1.0F,		0.0F, 0.0F, 0.0F,		0.0F, 0.0F,		0.0F, 1.0F, 0.0F,
	-1.0F, 0.0F, -1.0F,		0.0F, 0.0F, 0.0F,		0.0F, 1.0F,		0.0F, 1.0F, 0.0F,
	 1.0F, 0.0F, -1.0F,		0.0F, 0.0F, 0.0F,		1.0F, 1.0F,		0.0F, 1.0F, 0.0F,
	 1.0F, 0.0F,  1.0F,		0.0F, 0.0F, 0.0F,		1.0F, 0.0F,		0.0F, 1.0F, 0.0F,
    // clang-format on
};

constexpr GLuint indices[] = {
    // clang-format off
	0, 1, 2,
	0, 2, 3,
    // clang-format on
};

constexpr GLfloat lightVertices[] = {
    // clang-format off
    //     COORDINATES     //
	-0.1F, -0.1F,  0.1F,
	-0.1F, -0.1F, -0.1F,
	 0.1F, -0.1F, -0.1F,
	 0.1F, -0.1F,  0.1F,
	-0.1F,  0.1F,  0.1F,
	-0.1F,  0.1F, -0.1F,
	 0.1F,  0.1F, -0.1F,
	 0.1F,  0.1F,  0.1F,
    // clang-format on
};

constexpr GLuint lightIndices[] = {
    // clang-format off
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7,
    // clang-format on
};

void render(GLFWwindow* window,
            const Shader& shaderProgram,
            const Shader& lightShader,
            Camera& camera,
            const VAO& vao,
            const Texture& planksTex,
            const Texture& planksSpec,
            const VAO& lightVAO)
{
    glClearColor(0.07F, 0.13F, 0.17F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.inputs(window);
    camera.updateMatrix(45.0F, 0.1F, 100.0F);

    shaderProgram.activate();
    glUniform3f(glGetUniformLocation(shaderProgram.id, "cameraPosition"),
                camera.position.x,
                camera.position.y,
                camera.position.z);
    camera.matrix(shaderProgram, "cameraMatrix");
    planksTex.bind();
    planksSpec.bind();
    vao.bind();
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

    lightShader.activate();
    camera.matrix(lightShader, "camMatrix");
    lightVAO.bind();
    glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
};

void cleanup(GLFWwindow* window,
             const VAO& vao,
             const VBO& vbo,
             const EBO& ebo,
             const VAO& lightVAO,
             const VBO& lightVBO,
             const EBO& lightEBO,
             const Texture& planksTex,
             const Texture& planksSpec,
             const Shader& shaderProgram,
             const Shader& lightShader)
{
    vao.deleteVAO();
    vbo.deleteVBO();
    ebo.deleteEBO();
    planksTex.deleteTexture();
    planksSpec.deleteTexture();
    shaderProgram.deleteShader();
    lightVAO.deleteVAO();
    lightVBO.deleteVBO();
    lightEBO.deleteEBO();
    lightShader.deleteShader();
    glfwDestroyWindow(window);
    glfwTerminate();
}
} // namespace

namespace basic_shapes {
int showExampleWithTypesOfLight()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Example with types of light", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, width, height);

    Shader shaderProgram("shaders/example_with_types_of_light.vert",
                         "shaders/example_with_types_of_light.frag");
    VAO vao;
    vao.bind();
    VBO vbo(vertices, sizeof(vertices));
    EBO ebo(indices, sizeof(indices));
    VAO::linkAttrib(vbo, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
    VAO::linkAttrib(
        vbo, 1, 3, GL_FLOAT, 11 * sizeof(float), static_cast<char*>(nullptr) + (3 * sizeof(float)));
    VAO::linkAttrib(
        vbo, 2, 2, GL_FLOAT, 11 * sizeof(float), static_cast<char*>(nullptr) + (6 * sizeof(float)));
    VAO::linkAttrib(
        vbo, 3, 3, GL_FLOAT, 11 * sizeof(float), static_cast<char*>(nullptr) + (8 * sizeof(float)));
    VAO::unbind();
    VBO::unbind();
    EBO::unbind();

    Shader lightShader("shaders/light.vert", "shaders/light.frag");
    VAO lightVAO;
    lightVAO.bind();
    VBO lightVBO(lightVertices, sizeof(lightVertices));
    EBO lightEBO(lightIndices, sizeof(lightIndices));
    VAO::linkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    VAO::unbind();
    VBO::unbind();
    EBO::unbind();

    auto lightColor = glm::vec4(1.0F, 1.0F, 1.0F, 1.0F);
    auto lightPos = glm::vec3(0.5F, 0.5F, 0.5F);
    auto lightModel = glm::mat4(1.0F);
    lightModel = glm::translate(lightModel, lightPos);

    auto objectPos = glm::vec3(0.0F, 0.0F, 0.0F);
    auto objectModel = glm::mat4(1.0F);
    objectModel = glm::translate(objectModel, objectPos);

    lightShader.activate();
    glUniformMatrix4fv(
        glGetUniformLocation(lightShader.id, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.id, "lightColor"),
                lightColor.x,
                lightColor.y,
                lightColor.z,
                lightColor.w);
    shaderProgram.activate();
    glUniformMatrix4fv(
        glGetUniformLocation(shaderProgram.id, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
    glUniform4f(glGetUniformLocation(shaderProgram.id, "lightColor"),
                lightColor.x,
                lightColor.y,
                lightColor.z,
                lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.id, "lightPosition"),
                lightPos.x,
                lightPos.y,
                lightPos.z);

    Texture planksTex("resources/planks.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture::texUnit(shaderProgram, "tex0", 0);
    Texture planksSpec("resources/planksSpec.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
    Texture::texUnit(shaderProgram, "tex1", 1);

    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, glm::vec3(0.0F, 0.0F, 2.0F));

    while (!glfwWindowShouldClose(window)) {
        render(window, shaderProgram, lightShader, camera, vao, planksTex, planksSpec, lightVAO);
    }

    cleanup(window,
            vao,
            vbo,
            ebo,
            lightVAO,
            lightVBO,
            lightEBO,
            planksTex,
            planksSpec,
            shaderProgram,
            lightShader);
    return 0;
}
} // namespace basic_shapes
