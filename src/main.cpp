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

#include "shaders.h"
#include "ui.h"
#include <shaders/vao.h>
#include <shaders/vbo.h>
#include <shaders/ebo.h>
#include "textures.h"
#include "camera.h"
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

    int icon_width, icon_height, icon_channels; // this is the same system as textures lol
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

    GLfloat vertices[] =
    { //    COORDINATES         /  COLORS           / TEXTURE COORDS
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // Lower left
        -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   0.0f, 1.0f, // Upper left
        0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // Upper right
        0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Lower right
    };

    GLuint indices[] =
    {
        0, 2, 1, // Lower left triangle
        0, 3, 2, // Lower right triangle
    };

    GLfloat light_vertices[] = {

    };

    UI ui(window);

    Shader shader_program("shaders/default.vert", "shaders/default.frag");
    
    VAO vao1;
    vao1.Bind();

    VBO vbo1(vertices, sizeof(vertices));
    EBO ebo1(indices, sizeof(indices));

    vao1.LinkAttribute(vbo1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    vao1.LinkAttribute(vbo1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vao1.LinkAttribute(vbo1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    
    vao1.Unbind(); 
    vbo1.Unbind();
    ebo1.Unbind();

    Texture steamhappy_texture("steamhappy.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    steamhappy_texture.texture_unit(shader_program, "tex0", 0);

    Shader text_shader("shaders/text.vert", "shaders/text.frag");

    TextRenderer text_renderer_intro("fonts/din_light.ttf", 48, text_shader, width, height);
    TextRenderer text_renderer_engine("fonts/arial.ttf", 48, text_shader, width, height);

    glEnable(GL_DEPTH_TEST);

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
        camera.inputs(window);
        camera.matrix(45.0f, 0.1f, 100.0f, shader_program, "cam_matrix");

        vao1.Bind();
        steamhappy_texture.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
        vao1.Unbind();

        text_renderer_engine.render("SilverBin Editor Debug Text:tm:", 10.0f, 10.0f, 1.0f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

        calculate_fps();

        std::string fps_text = "FPS: " + std::to_string(current_fps);
        
        text_renderer_engine.render(
            fps_text,
            10.0f, 550.0f,
            0.7f,
            glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
        );

        ui.begin();
        ui.render();
        ui.end();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vao1.Delete();
    vbo1.Delete();
    ebo1.Delete();
    shader_program.Delete();
    steamhappy_texture.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}