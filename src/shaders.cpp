#include "shaders.h"
#include <iostream>

std::string load_shader_src(const char* file_name) {
    std::ifstream in(file_name, std::ios::binary);
    if (in) {
        std::string file_content;
        in.seekg(0, std::ios::end);
        file_content.resize(in.tellg());

        in.seekg(0, std::ios::beg);
        in.read(&file_content[0], file_content.size());
        in.close();

        return file_content;
    }

    throw(errno);
}

void check_compile_errors(GLuint shader, std::string type) {
    int success;
    char info_log[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, info_log);
            std::cout << "SHADER COMPILATION ERROR: " << type << "\n"
                      << info_log << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, info_log);
            std::cout << "PROGRAM LINKING ERROR: " << type << "\n"
                      << info_log << std::endl;
        }
    }
}

Shader::Shader(const char* vertex_file, const char* frag_file) {
    std::string vertex_code = load_shader_src(vertex_file);
    std::string frag_code = load_shader_src(frag_file);

    const char* vertex_shader_source = vertex_code.c_str();
    const char* frag_shader_source = frag_code.c_str();

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    check_compile_errors(vertex_shader, "VERTEX");

    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_shader_source, NULL);
    glCompileShader(frag_shader);
    check_compile_errors(frag_shader, "FRAGMENT");

    id = glCreateProgram();

    glAttachShader(id, vertex_shader);
    glAttachShader(id, frag_shader);
    glLinkProgram(id);

    check_compile_errors(id, "PROGRAM");

    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);
}

void Shader::Activate() {
    glUseProgram(id);
}

void Shader::Delete() {
    glDeleteProgram(id);
}

void Shader::setInt(const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) {
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value));
}