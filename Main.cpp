#include <iostream>
#include <string>
#include <windows.h>
#include <glew.h>
#include <GL/freeglut.h> 
#include <GL/glut.h>
#include <GL/gl.h> 
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shader.h"
#include "stdio.h"

using namespace std;
using namespace glm;

Shader shader;
void DrawObject();
float getSimulationTime();
void input(float);

float moveSpeed = 1.0f;
vec4 offset;

void Display(void)
{
    // ������� ����� �����
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // �������� ���� ������� (�� ������ ������)
    glEnable(GL_DEPTH_TEST);
    // ���������� ������
    shader.activate();

    // ������������� �������� uniform-���������� ��� �����
    vec4 color1 = vec4(0.0, 0.0, 1.0, 1.0);
    shader.setUniform("color1", color1);
    vec4 color2 = vec4(1.0, 0.0, 0.0, 1.0);
    shader.setUniform("color2", color2);

    shader.setUniform("offset", offset);

    // ������� �������������
    DrawObject();
    // ����� ��������� � ������� �������
    glutSwapBuffers();
};
// �������, ���������� ��� ��������� �������� ����
void Reshape(int w, int h)
{
    // ���������� ����� ������� ���������, ������ ���� ������� ����
    glViewport(0, 0, w, h);
};


// ������� ���������� ����� ��������� �����������
void Simulation()
{
    input(getSimulationTime());
    glutPostRedisplay();
    glutIdleFunc(Simulation);
}

void main(int argc, char** argv)
{
    // ������������� ���������� GLUT
    glutInit(&argc, argv);
    // ������������� ������� (������ ������)
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
    // ���������� � ������ OpenGL (������ 3.3 ��� ��������� �������� �������������)
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    // ������������� ������� ����� ���� ����
    glutInitWindowPosition(300, 100);
    // ������������� ������ ����
    glutInitWindowSize(800, 600);
    // �������� ����
    glutCreateWindow("laba_01");
    // ������������� GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "Glew error: %s\n", glewGetErrorString(err));
        return;
    }
    // ����������� ������� ������ OpenGL
    printf("OpenGL Version = %s\n\n", glGetString(GL_VERSION));
    // �������� �������
    shader.load("Shaders\\Example.vsh", "Shaders\\Example.fsh");
    // ������������� �������, ������� ����� ���������� ��� ����������� ����
    glutDisplayFunc(Display);
    // ������������� �������, ������� ����� ���������� ��� ��������� �������� ����
    glutReshapeFunc(Reshape);
    // ������������� ������� ������� ���������� ������ ���, ����� ��������� �����������
    glutIdleFunc(Simulation);

    // �������� ���� ��������� ��������� ��
    glutMainLoop();
    return;
}
// ������� ��� ������ ��������������
void DrawObject() {
    // ���������� ��� ������ ������� (�������������� �� ���� �������������)
    static bool init = true;
    static GLuint VAO_Index = 0; // ������ VAO-������
    static GLuint VBO_Index = 0; // ������ VBO-������
    static int VertexCount = 0; // ���������� ������
    // ��� ������ ������ �������������� VBO � VAO
    if (init) {
        init = false;
        // �������� � ���������� VBO
        glGenBuffers(1, &VBO_Index);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
        GLfloat Verteces[] = {
        -0.5, 0.5,
        -0.5, -0.5,
        0.5, 0.5,
        0.5, 0.5,
        -0.5, -0.5,
        0.5, -0.5
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(Verteces), Verteces, GL_STATIC_DRAW);
        // �������� VAO
        glGenVertexArrays(1, &VAO_Index);
        glBindVertexArray(VAO_Index);
        // ���������� VAO
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
        // ���������� ������
        VertexCount = 6;
    }
    // ������� �������������
    glBindVertexArray(VAO_Index);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    return;
}

// �������� deltaTime
float getSimulationTime()
{
    static LARGE_INTEGER lastValue;
    LARGE_INTEGER newValue;
    QueryPerformanceCounter(&newValue);

    LARGE_INTEGER Freq;
    QueryPerformanceFrequency(&Freq);

    float delta = newValue.QuadPart - lastValue.QuadPart;
    lastValue = newValue;

    return delta / Freq.QuadPart;
};

// ���������� ��������������� � ������� WASD
void input(float deltaTime) 
{
    if (GetAsyncKeyState(87))  // W
    {
        if (offset.y < 0.5)
        {
            offset.y += moveSpeed * deltaTime;
        }
        else offset.y = 0.5;
    }

    if (GetAsyncKeyState(83))  // S
    {
        if (offset.y > -0.5)
        {
            offset.y -= moveSpeed * deltaTime;
        }
        else offset.y = -0.5;
    }

    if (GetAsyncKeyState(68)) // D
    {
        if (offset.x < 0.5)
        {
            offset.x += moveSpeed * deltaTime;
        }
        else offset.x = 0.5;
    }

    if (GetAsyncKeyState(65)) // A
    {
        if (offset.x > -0.5)
        {
            offset.x -= moveSpeed * deltaTime;
        }
        else offset.x = -0.5;
    }
}
