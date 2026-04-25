#include "ui.h"

UI::UI(GLFWwindow* window) {
    this->window = window;

    ImGui::CreateContext();
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    std::cout << "imgui loaded" << std::endl;
}

void UI::begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI::render() {
    ImGui::Begin("debug");
    ImGui::Text("hi from imgui");

    if (ImGui::Button("Click me")) {
        std::cout << "button pressed\n";
    }

    ImGui::End();
}

void UI::end() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}