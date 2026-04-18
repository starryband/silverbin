#include <shaders/vao.h>

VAO::VAO() {
    glGenVertexArrays(1, &id);
}

void VAO::Link(VBO vbo, GLuint layout) {
    vbo.Bind();

    glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    vbo.Unbind();
}

void VAO::Bind() {
    glBindVertexArray(id);
}

void VAO::Unbind() {
    glBindVertexArray(0);
}

void VAO::Delete() {
    glDeleteVertexArrays(1, &id);
}