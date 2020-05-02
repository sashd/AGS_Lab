#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include <glew.h>
#include <GL/freeglut.h> 
#include <GL/glut.h>
#include <GL/gl.h> 
#include "glm/glm.hpp"
#include <map>

class Shader
{
public:
    // �������� ������� �� ������� ������
    bool load(std::string vertexShaderFilename, std::string fragmentShaderFilename);

    // ������������ ��������� ���������
    void activate();

    // �������������� ��������� ���������
    void deactivate();

    // ��������� uniform-����������
    void setUniform(std::string name, glm::vec4& value);

    // ��������� uniform-����������
    void setUniform(std::string name, glm::mat4& value);

private:
    // ��������� ��������� (������)
    GLuint program;
    // ��������� ������
    GLuint shader;
    std::map<std::string, GLuint>uniforms; // ������ ��� ���������� � �� location
    // �������� ���������� ������� � �������� ������ ������� �� �����
    GLuint createShaderObject(GLenum shaderType, std::string filename);
    // ��������� ������� (location) uniform-����������
    GLuint getUniformLocation(std::string name);
};

