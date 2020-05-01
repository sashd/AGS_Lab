#include "Shader.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <glew.h>
#include <GL/freeglut.h> 
#include <GL/glut.h>
#include <GL/gl.h> 
#include "glm/glm.hpp"
#include "Shader.h"

using namespace std;

bool Shader::load(string vertexShaderFilename, string fragmentShaderFilename)
{
    // C������ ��������� ���������
    program = glCreateProgram();

    // ��������� ��������� ������
    createShaderObject(GL_VERTEX_SHADER, vertexShaderFilename);
    glAttachShader(program, shader);

    // ����������� ��������� ������
    createShaderObject(GL_FRAGMENT_SHADER, fragmentShaderFilename);
    glAttachShader(program, shader);

    // ��������
    glLinkProgram(program);
    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE)
    {
        char errorMessage[300];
        GLsizei errorMsgLen;
        glGetProgramInfoLog(program, 300, &errorMsgLen, errorMessage);
        cout << errorMessage << endl;
        return false;
    }
    else 
    {
        cout << "Link process successful" << endl;
    }
}

GLuint Shader::createShaderObject(GLenum shaderType, std::string filename)
{
    shader = glCreateShader(shaderType);
    // ���������������, ��� � ��������� ��������� �� ����� 10000 ������
    char source[10000];

    ifstream f(filename);
    if (!f.is_open())
    {
        cout << "Can't open file " << filename << endl;
        return false;
    }

    f.getline(source, 10000, 0);
    // �������� ������ ������� � ��������� ������
    char* src[1] = { source };

    glShaderSource(shader, 1, (const char**)src, NULL);
    glCompileShader(shader);
    GLint compileStatus = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus != GL_TRUE)
    {
        char errorMessage[300];
        GLsizei errorMsgLen;
        glGetShaderInfoLog(shader, 300, &errorMsgLen, errorMessage);
        cout << errorMessage << endl;
        return false;
    }
}

void Shader::activate()
{
    glUseProgram(program);
}

void Shader::deactivate()
{
    glUseProgram(0);
}

void Shader::setUniform(std::string name, glm::vec4& value)
{
    GLint location = getUniformLocation(name);
    glUniform4f(location, value.r, value.g, value.b, value.a);
    this->uniforms[name] = location;
}

GLuint Shader::getUniformLocation(std::string name)
{
    return glGetUniformLocation(program, name.c_str());
}

