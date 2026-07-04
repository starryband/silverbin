#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::string load_shader_src(const char* file_name);

class Shader {
    public:
        GLuint id;
        Shader(const char* vertex_file, const char* frag_file);

        void Activate();
        void Delete();

        void setInt(const std::string &name, int value);
        void setFloat(const std::string &name, float value);
        void setMat4(const std::string &name, const glm::mat4 &mat);
        void setVec3(const std::string &name, const glm::vec3 &value);
};

#endif