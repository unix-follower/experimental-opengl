#include "experimental_opengl.hpp"
#include <cmath>
#include <iostream>

struct ShaderPair {
    GLuint vertex, fragment;
};

const char* vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
                                   "}\n\0";

ShaderPair createTriangleShaders()
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    return {vertexShader, fragmentShader};
}

GLuint createTriangleShaderProgram()
{
    GLuint shaderProgram = glCreateProgram();
    auto shaders = createTriangleShaders();
    glAttachShader(shaderProgram, shaders.vertex);
    glAttachShader(shaderProgram, shaders.fragment);
    glLinkProgram(shaderProgram);

    glDeleteShader(shaders.vertex);
    glDeleteShader(shaders.fragment);

    return shaderProgram;
}

void bindBuffers(GLuint& vertexArrayObject,
                 GLuint& vertexBufferObject,
                 const GLfloat* vertices,
                 size_t verticesSize)
{
    glGenVertexArrays(1, &vertexArrayObject);
    glGenBuffers(1, &vertexBufferObject);

    glBindVertexArray(vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void drawTriangle(GLuint shaderProgram, GLuint vertexArrayObject)
{
    glClearColor(0.07F, 0.13F, 0.17F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(vertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void cleanUpShaderProgram(GLuint shaderProgram, GLuint vertexArrayObject, GLuint vertexBufferObject)
{
    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteProgram(shaderProgram);
}

int showTriangle()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Experimental OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, 800, 800);

    GLuint shaderProgram = createTriangleShaderProgram();

    GLfloat vertices[] = {
        -0.5F,
        -0.5F * float(sqrt(3)) / 3,
        0.0F, // Lower left corner
        0.5F,
        -0.5F * float(sqrt(3)) / 3,
        0.0F, // Lower right corner
        0.0F,
        0.5F * float(sqrt(3)) * 2 / 3,
        0.0F // Upper corner
    };

    GLuint vao;
    GLuint vbo;
    bindBuffers(vao, vbo, vertices, sizeof(vertices));

    while (!glfwWindowShouldClose(window)) {
        drawTriangle(shaderProgram, vao);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanUpShaderProgram(shaderProgram, vao, vbo);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
