#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

#include "shaders.h"
#include <shaders/vao.h>
#include <shaders/vbo.h>
#include <shaders/ebo.h>
#include "textures.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
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

    window = glfwCreateWindow(800, 600, "SilverBin Editor", NULL, NULL);
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
    glViewport(0, 0, 800, 600);
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

    GLuint scale_uni = glGetUniformLocation(shader_program.id, "scale");
    glUniform1f(scale_uni, 0.0f);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_program.Activate();
        vao1.Bind();
        steamhappy_texture.Bind();
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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