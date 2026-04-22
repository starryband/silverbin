#ifndef TEXT_H
#define TEXT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <freetype/ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <string>
#include "shaders.h"
#include <shaders/vao.h>

struct Character {
    GLuint texture_id;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint advance;
};

class TextRenderer {
    public:
        std::map<char, Character> characters;
        Shader& shader;
        VAO vao;
        GLuint vao_id, vbo_id;

        TextRenderer(const std::string& font_path, unsigned int font_size, Shader& shader, unsigned int width, unsigned int height);
        void render(const std::string& text, float x, float y, float scale, glm::vec4 color);
};

#endif