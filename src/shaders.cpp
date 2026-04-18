#include "shaders.h"

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

    throw (errno);
}

Shader::Shader(const char* vertex_file, const char* frag_file) {
    std::string vertex_code = load_shader_src(vertex_file);
    std::string frag_code = load_shader_src(frag_file);

    const char* vertex_shader_source = vertex_code.c_str();
    const char* frag_shader_source = frag_code.c_str();

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_shader_source, NULL);
    glCompileShader(frag_shader);

    id = glCreateProgram();

    glAttachShader(id, vertex_shader);
    glAttachShader(id, frag_shader);
    glLinkProgram(id);

    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);
}

void Shader::Activate() {
    glUseProgram(id);
}

void Shader::Delete() {
    glDeleteProgram(id);
}