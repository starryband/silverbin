#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shaders.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture_Data {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture_Data> textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture_Data> textures);
        void Draw(Shader &shader);
    private:
        unsigned int vao, vbo, ebo;
        void setup_mesh();
};

#endif