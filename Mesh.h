#pragma once

#include <glew.h>
#include <GL/freeglut.h> 
#include <GL/glut.h>
#include <GL/gl.h> 
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm\gtx\transform.hpp"

#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <map>

struct Vertex
{
    float coord[3];
    float normal[3];
    float texCoord[2];
};

class Mesh
{
public:
    // �����������s
    Mesh();
    Mesh(std::string);
    ~Mesh();

    // �������� ���� �� �������� obj-�����
    bool load(std::string);
    // ����� ����
    void draw();

private:
    int vertexCount;
    GLuint vao;
    GLuint buffers[2];
};