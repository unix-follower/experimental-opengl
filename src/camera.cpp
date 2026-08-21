#include "camera.hpp"

Camera::Camera(int width, int height, glm::vec3 position)
{
    this->width = width;
    this->height = height;
    this->position = position;
}

void Camera::matrix(float fovDegrees,
                    float nearPlane,
                    float farPlane,
                    const Shader& shader,
                    const char* uniform) const
{
    auto view = glm::mat4(1.0F);
    auto projection = glm::mat4(1.0F);

    view = glm::lookAt(position, position + orientation, up);
    projection = glm::perspective(
        glm::radians(fovDegrees), (float)width / static_cast<float>(height), nearPlane, farPlane);

    glUniformMatrix4fv(
        glGetUniformLocation(shader.id, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::inputs(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += speed * orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position += speed * -glm::normalize(glm::cross(orientation, up));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position += speed * -orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += speed * glm::normalize(glm::cross(orientation, up));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        position += speed * up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        position += speed * -up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        speed = 0.4F;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        speed = 0.1F;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (firstClick) {
            glfwSetCursorPos(window, (width / 2.0), (height / 2.0));
            firstClick = false;
        }

        double mouseX;
        double mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float rotationX =
            sensitivity * (float)(mouseY - (height / 2.0)) / static_cast<float>(height);
        float rotationY = sensitivity * (float)(mouseX - (width / 2.0)) / static_cast<float>(width);

        if (glm::vec3 newOrientation = glm::rotate(
                orientation, glm::radians(-rotationX), glm::normalize(glm::cross(orientation, up)));
            abs(glm::angle(newOrientation, up) - glm::radians(90.0F)) <= glm::radians(85.0F)) {
            orientation = newOrientation;
        }

        orientation = glm::rotate(orientation, glm::radians(-rotationY), up);

        glfwSetCursorPos(window, (width / 2.0), (height / 2.0));
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }
}
