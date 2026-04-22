#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shaders.h"

class Camera {
    public:
        glm::vec3 position;
        glm::vec3 orientation = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

        int width;
        int height;

        bool focused = true;
        bool first_click = true;

        float speed = 0.1f;
        float sensitivity = 200.0f;

        Camera(int width, int height, glm::vec3 _position);

        void matrix(float fov_degrees, float near_plane, float far_plane, Shader& shader, const char* uniform);
        void inputs(GLFWwindow* window);
};

#endif