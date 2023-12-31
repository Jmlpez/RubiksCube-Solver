#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include "../include/VBO.h"

class VAO
{
public:
    GLuint ID;
    VAO();

    // Links a VBO attribute to the VAO using a certain layout
    void LinkAttrib(VBO &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void *offset);
    void Bind();
    void Unbind();
    void Delete();
};

#endif
