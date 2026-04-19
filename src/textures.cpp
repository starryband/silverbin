#include <iostream>
#include <textures.h>

Texture::Texture(const char* image, GLenum texture_type, GLenum slot, GLenum format, GLenum pixel_type) {
    type = texture_type;

    int width_img, height_img, num_color_channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load("steamhappy.png", &width_img, &height_img, &num_color_channels, 4);

    GLuint texture;
    glGenTextures(1, &id);
    glActiveTexture(slot);
    glBindTexture(texture_type, id);

    glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(texture_type, 0, GL_RGBA, width_img, height_img, 0, format, pixel_type, bytes);
    glGenerateMipmap(texture_type);

    stbi_image_free(bytes);
    glBindTexture(texture_type, 0);
}

void Texture::texture_unit(Shader& shader, const char* uniform, GLuint unit) {
    GLuint texture_unit = glGetUniformLocation(shader.id, uniform);

    shader.Activate();
    glUniform1i(texture_unit, unit);
}

void Texture::Bind() {
    glBindTexture(type, id);
}

void Texture::Unbind() {
    glBindTexture(type, 0);
}

void Texture::Delete() {
    glDeleteTextures(1, &id);
}