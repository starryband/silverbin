#ifndef UI_H
#define UI_H

#include <iostream>
#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class UI {
    public:
        UI(GLFWwindow* window);

        void begin();
        void render();
        void end();

    private:
        GLFWwindow* window;
};

#endif