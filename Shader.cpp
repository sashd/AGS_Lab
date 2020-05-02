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
using namespace glm;

bool Shader::load(string vertexShaderFilename, string fragmentShaderFilename)
{
    // C������ ��������� ���������
    program = glCreateProgram();


    // ����������� ��������� ������
    shader = createShaderObject(GL_FRAGMENT_SHADER, fragmentShaderFilename);
    glAttachShader(program, shader);

    // ��������� ��������� ������
    shader = createShaderObject(GL_VERTEX_SHADER, vertexShaderFilename);
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
    GLuint sh = glCreateShader(shaderType);
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

    glShaderSource(sh, 1, (const char**)src, NULL);
    glCompileShader(sh);
    GLint compileStatus;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus != GL_TRUE)
    {
        char errorMessage[300];
        GLsizei errorMsgLen;
        glGetShaderInfoLog(sh, 300, &errorMsgLen, errorMessage);
        cout << errorMessage << endl;
        return false;
    }
    else 
    {
        return sh;
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
    GLint location = glGetUniformLocation(program, name.c_str());
    glUniform4f(location, value.r, value.g, value.b, value.a);
}

