#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include "vbo.h"

class VAO {
    public:
        GLuint id;
        VAO();

        void LinkAttribute(VBO& vbo, GLuint layout, GLuint num_components, GLenum type, GLsizeiptr stride, void* offset);
        void Bind();
        void Unbind();
        void Delete();
};

#endif