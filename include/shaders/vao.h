#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include "vbo.h"

class VAO {
    public:
        GLuint id;
        VAO();

        void Link(VBO vbo, GLuint layout);
        void Bind();
        void Unbind();
        void Delete();
};

#endif