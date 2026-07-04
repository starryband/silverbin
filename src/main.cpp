#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>

#include "shaders.h"
#include "ui.h"
#include "mesh.h"
#include "textures.h"
#include "camera.h"
#include "model.h"
#include <text.h>
#include <engine_events/intro.h>

const unsigned int width = 800;
const unsigned int height = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

double previous_time = 0.0;
int frame_count = 0;
int current_fps = 0;

void calculate_fps() {
    double current_time = glfwGetTime();
    frame_count++;

    if (current_time - previous_time >= 1.0) {
        current_fps = frame_count;
        frame_count = 0;
        previous_time = current_time;
    }
}

int main() {
    if (!glfwInit()) {
        std::cout << "failed to initialize glfw" << std::endl;
        return -1;
    }
 
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;

    window = glfwCreateWindow(width, height, "SilverBin", NULL, NULL);
    if (window == NULL) {
        std::cout << "failed to open/create glfw window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "failed to initialize glad" << std::endl;
        return -1;
    }

    int icon_width, icon_height, icon_channels;
    unsigned char* icon_pixels = stbi_load("icon.png", &icon_width, &icon_height, &icon_channels, 4);

    if (icon_pixels) {
        GLFWimage icon[1];
        icon[0].width = icon_width;
        icon[0].height = icon_height;
        icon[0].pixels = icon_pixels;

        glfwSetWindowIcon(window, 1, icon);
        stbi_image_free(icon_pixels);
    } else {
        std::cout << "no icon pixels" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    UI ui(window);

    Shader shader_program("shaders/default.vert", "shaders/default.frag");

    Texture steamhappy_texture("steamhappy.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    steamhappy_texture.texture_unit(shader_program, "tex0", 0);

    std::vector<Vertex> vertices = {
        { glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) }, // Lower left
        { glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) }, // Upper left
        { glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) }, // Upper right
        { glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) }, // Lower right
    };

    std::vector<unsigned int> indices = { 0, 2, 1, 0, 3, 2 };
    std::vector<Texture_Data> textures = {
        { steamhappy_texture.id, "texture_diffuse" }
    };

    // Mesh quad(vertices, indices, textures);

    Model test_cube("models/test_cube/test_cube.obj");

    Shader text_shader("shaders/text.vert", "shaders/text.frag");

    TextRenderer text_renderer_intro("fonts/din_light.ttf", 48, text_shader, width, height);
    TextRenderer text_renderer_engine("fonts/arial.ttf", 48, text_shader, width, height);

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 0.0f));

    GLuint scale_uni = glGetUniformLocation(shader_program.id, "scale");
    glUniform1f(scale_uni, 0.0f);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Intro intro({
        {"Made with Silver", 0.5f, 1.0f},
    });

    float last_frame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float current_frame = glfwGetTime();
        float delta_time = current_frame - last_frame;

        last_frame = current_frame;

        if (!intro.done) {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            intro.update(delta_time);
            intro.render(text_renderer_intro, width, height);

            glfwSwapBuffers(window);
            glfwPollEvents();

            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                intro.done = true;
            }

            continue;
        }

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_program.Activate();

        glm::vec3 lightPos(2.0f, 2.0f, 2.0f);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

        shader_program.setVec3("lightPos", lightPos);
        shader_program.setVec3("lightColor", lightColor);
        shader_program.setVec3("viewPos", camera.position);

        glm::mat4 model = glm::mat4(1.0f);
        shader_program.setMat4("model", model);

        camera.inputs(window);
        camera.matrix(45.0f, 0.1f, 100.0f, shader_program, "cam_matrix");

        test_cube.Draw(shader_program);

        text_renderer_engine.render("SilverBin Editor Debug Text:tm:", 10.0f, 10.0f, 1.0f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

        /*
        std::cout << glGetUniformLocation(shader_program.id, "lightPos") << std::endl;
        std::cout << glGetUniformLocation(shader_program.id, "texture_diffuse1") << std::endl;
        */

        calculate_fps();

        std::string fps_text = "FPS: " + std::to_string(current_fps);
        text_renderer_engine.render(fps_text, 10.0f, 550.0f, 0.7f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

        ui.begin();
        ui.render();
        ui.end();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shader_program.Delete();
    steamhappy_texture.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}