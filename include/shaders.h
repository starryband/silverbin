#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>

std::string load_shader_src(const char* file_name);

class Shader {
    public:
        GLuint id;
        Shader(const char* vertex_file, const char* frag_file);

        void Activate();
        void Delete();
};

#endif