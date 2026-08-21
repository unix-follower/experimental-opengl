#pragma once

#include <glad/glad.h>
// clang-format off
#include <GLFW/glfw3.h>
// clang-format on

#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shader.hpp"

class Camera {
  public:
    glm::vec3 position;
    glm::vec3 orientation = glm::vec3(0.0F, 0.0F, -1.0F);
    glm::vec3 up = glm::vec3(0.0F, 1.0F, 0.0F);

    bool firstClick = true;

    int width;
    int height;

    float speed = 0.1F;
    float sensitivity = 100.0F;

    Camera(int width, int height, glm::vec3 position);

    void matrix(float fovDegrees,
                float nearPlane,
                float farPlane,
                const Shader& shader,
                const char* uniform) const;
    void inputs(GLFWwindow* window);
};
