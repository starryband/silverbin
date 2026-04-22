#include "camera.h"

Camera::Camera(int width, int height, glm::vec3 position) {
    Camera::width = width;
    Camera::height = height;
    Camera::position = position;
}

void Camera::matrix(float fov_degrees, float near_plane, float far_plane, Shader& shader, const char* uniform) {
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = glm::lookAt(position, position + orientation, up);
    projection = glm::perspective(glm::radians(fov_degrees), (float)width / (float)height, near_plane, far_plane);

    glUniformMatrix4fv(glGetUniformLocation(shader.id, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::inputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        focused = false;
        first_click = true;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !focused) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        focused = true;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += speed * orientation;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position += speed * -glm::normalize(glm::cross(orientation, up));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position += speed * -orientation;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += speed * glm::normalize(glm::cross(orientation, up));
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        position += speed * up;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        position += speed * -up;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        speed = 0.05f;
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
        speed = 0.1f;

    if (!focused)
        return;

    double mouse_x, mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    if (first_click) {
        glfwSetCursorPos(window, width / 2.0, height / 2.0);
        first_click = false;
        return;
    }

    float rot_x = sensitivity * (float)(mouse_y - height / 2.0) / height;
    float rot_y = sensitivity * (float)(mouse_x - width  / 2.0) / width;

    glm::vec3 new_orientation = glm::rotate(orientation, glm::radians(-rot_x), glm::normalize(glm::cross(orientation, up)));

    if (glm::angle(new_orientation, up) > glm::radians(5.0f) && glm::angle(new_orientation, up) < glm::radians(175.0f)) {
        orientation = new_orientation;
    }

    orientation = glm::rotate(orientation, glm::radians(-rot_y), up);

    glfwSetCursorPos(window, width / 2.0, height / 2.0);
}