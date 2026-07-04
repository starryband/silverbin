#include "ui.h"
#include "engine_events/functions/open_file_dialog.h"
#include "model.h"

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
    ImGui::Text("Silver Tools");

    if (ImGui::Button("Load Model")) {
        std::wstring path = FileDialogs::OpenFileDialog();
        if (!path.empty()) {
            std::cout << "found file" << std::endl;
        }
    }

    ImGui::End();
}

void UI::end() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}