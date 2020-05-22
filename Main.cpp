#include <windows.h>
#include <iostream>
#include "stdio.h"
#include <string>
#include <glew.h>
#include <GL/freeglut.h> 
#include <GL/glut.h>
#include <GL/gl.h> 
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "ResourceManager.h"
#include "GraphicObject.h"

using namespace std;
using namespace glm;

void Display(void);
void Reshape(int w, int h);
void Simulation();
void drawCube();
float getSimulationTime();
void input(float);
void mouseWheel(int wheel, int direction, int x, int y);
void initGraphicObjects();

Shader shader;
Camera camera;
float w = 800.0f, h = 600.0f; // ���������� ������
float moveSpeed = 0.05f; // camera move speed
float scrollSpeed = 0.5f; // camera zoom speed
float mouse_sensitivity = 50; // camera rotate speed
POINT oldCursPos;
POINT newCursPos;

vector<GraphicObject>graphicObjects;

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
    glutInitWindowPosition(300, 300);

    // ������������� ������ ����
    glutInitWindowSize(w, h);

    // �������� ����
    glutCreateWindow("LAB#3");

    // ������������� GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "Glew error: %s\n", glewGetErrorString(err));
        return;
    }

    // ����������� ������� ������ OpenGL
    printf("OpenGL Version = %s\n\n", glGetString(GL_VERSION));

    // ��������� ������� ������
    camera.setPosition(vec3(10, 1, 1));

    // �������� �������
    shader.load("Shaders\\Example.vsh", "Shaders\\Example.fsh");

    // ������������� ����������� ��������
    initGraphicObjects();

    // ������������� �������, ������� ����� ���������� ��� ����������� ����
    glutDisplayFunc(Display);
    // ������������� �������, ������� ����� ���������� ��� ��������� �������� ����
    glutReshapeFunc(Reshape);
    // ������������� ������� ������� ���������� ������ ���, ����� ��������� �����������
    glutIdleFunc(Simulation);
    // ������� �������� ����
    glutMouseWheelFunc(mouseWheel);

    // �������� ���� ��������� ��������� ��
    glutMainLoop();
    return;
}

void Display(void)
{
    // ������� ����� �����
    glClearColor(0.7, 0.7, 0.7, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // �������� ���� ������� (�� ������ ������)
    glEnable(GL_DEPTH_TEST);

    // ��������� �������������� ��������� ����������
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // ���������� ������, ������������ ��� ������ �������
    shader.activate();

    // ������������� ������� ��������
    mat4& projectionMatrix = camera.getProjectionMatrix();
    shader.setUniform("projectionMatrix", projectionMatrix);

    // �������� ������� ������
    mat4& viewMatrix = camera.getViewMatrix();

    // ������� ��� �������
    for (int i = 0; i < graphicObjects.size(); i++) {
        // ������������� ������� ���������� ������
        mat4 modelViewMatrix = viewMatrix * graphicObjects[i].getModelMatrix();
        shader.setUniform("modelViewMatrix", modelViewMatrix);
        // ������������� ����
        shader.setUniform("color", graphicObjects[i].getColor());
        // ������� ���
        int meshId = graphicObjects[i].getMeshId();
        Mesh* mesh = ResourceManager::instance().getMesh(meshId);
        if (mesh != nullptr) mesh->draw();
    }

    // ������ ������
    glutSwapBuffers();
}

// �������, ���������� ��� ��������� �������� ����
void Reshape(int w, int h)
{
    // ���������� ����� ������� ���������, ������ ���� ������� ����
    glViewport(0, 0, w, h);
    // ������������� ������� ��������
    camera.setProjectionMatrix(radians(45.0f), float(w) / h, 1.0f, 100.0f);
}

// ������� ���������� ����� ��������� �����������
void Simulation()
{
    input(getSimulationTime());
    glutPostRedisplay();
    glutIdleFunc(Simulation);
}

// ����� ����
void drawCube()
{
    // ���������� ��� ������ ������� (�������������� �� ���� �������������)
    static GLuint VAO_Index = 0; // ������ VAO-������
    static GLuint VBO_Index = 0; // ������ VBO-������
    static int VertexCount = 0; // ���������� ������
    static bool init = true;
    if (init) {
        // �������� � ���������� VBO
        glGenBuffers(1, &VBO_Index);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
        GLfloat Verteces[] = {
            // �������� ����� (��� ������������)
            -0.5, +0.5, +0.5, -0.5, -0.5, +0.5, +0.5, +0.5, +0.5,
            +0.5, +0.5, +0.5, -0.5, -0.5, +0.5, +0.5, -0.5, +0.5,
            // ������ ����� (��� ������������)
            +0.5, +0.5, -0.5, +0.5, -0.5, -0.5, -0.5, +0.5, -0.5,
            -0.5, +0.5, -0.5, +0.5, -0.5, -0.5, -0.5, -0.5, -0.5,
            // ������ ����� (��� ������������)
            +0.5, -0.5, +0.5, +0.5, -0.5, -0.5, +0.5, +0.5, +0.5,
            +0.5, +0.5, +0.5, +0.5, -0.5, -0.5, +0.5, +0.5, -0.5,
            // ����� ����� (��� ������������)
            -0.5, +0.5, +0.5, -0.5, +0.5, -0.5, -0.5, -0.5, +0.5,
            -0.5, -0.5, +0.5, -0.5, +0.5, -0.5, -0.5, -0.5, -0.5,
            // ������� ����� (��� ������������)
            -0.5, +0.5, -0.5, -0.5, +0.5, +0.5, +0.5, +0.5, -0.5,
            +0.5, +0.5, -0.5, -0.5, +0.5, +0.5, +0.5, +0.5, +0.5,
            // ������ ����� (��� ������������)
            -0.5, -0.5, +0.5, -0.5, -0.5, -0.5, +0.5, -0.5, +0.5,
            +0.5, -0.5, +0.5, -0.5, -0.5, -0.5, +0.5, -0.5, -0.5
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(Verteces), Verteces, GL_STATIC_DRAW);
        // �������� VAO
        glGenVertexArrays(1, &VAO_Index);
        glBindVertexArray(VAO_Index);
        // ������������� VAO
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
        int location = 0;
        glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(location);

        // "�������" ������ VAO
        glBindVertexArray(0);

        // �������� ���������� ������
        VertexCount = 6 * 6;
        init = false;
    }
    // ����� ������ �� �����
    glBindVertexArray(VAO_Index);
    glDrawArrays(GL_TRIANGLES, 0, VertexCount);
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

void input(float deltaTime) 
{
    if (GetAsyncKeyState(VK_UP))
    {
        camera.move(moveSpeed, 0);
    }

    if (GetAsyncKeyState(VK_DOWN))
    {
        camera.move(-moveSpeed, 0);
    }

    if (GetAsyncKeyState(VK_RIGHT))
    {
        camera.move(0, -moveSpeed);
    }

    if (GetAsyncKeyState(VK_LEFT)) 
    {
        camera.move(0, moveSpeed);
    }

    if (GetAsyncKeyState(VK_RBUTTON))
    {
        GetCursorPos(&newCursPos);
        int dx = newCursPos.x - oldCursPos.x;
        int dy = newCursPos.y - oldCursPos.y;

        camera.rotate(dx * mouse_sensitivity * deltaTime, dy * mouse_sensitivity * deltaTime);

        oldCursPos = newCursPos;
    }
    else
    {
        oldCursPos = newCursPos;
        GetCursorPos(&newCursPos);
    }

}

// ������� ��������� �������� �������� �����
void mouseWheel(int wheel, int direction, int x, int y)
{
    // ����������, �� ������� ���������� ����������/������� ������
    float delta = -direction * scrollSpeed;
    camera.zoom(delta);
}

// ��������������� ������� ��� ������������� ����������� ��������
void initGraphicObjects()
{
    // ������ �� �������� �������� (��� ��������)
    ResourceManager& rm = ResourceManager::instance();
    // ��������� ���������� ��� �������� ��������������� ����
    int meshId = -1;
    // ��������� ���������� ��� ������������� ������������ �������
    GraphicObject graphicObject;
    // ���������� ������������ �������
    meshId = rm.loadMesh("meshes/buildings/house_2.obj");
    graphicObject.setMesh(meshId);
    graphicObject.setColor(vec4(0.2f, 0.2f, 0.2f, 1.0f));
    graphicObject.setPosition(vec3(0, 0, 0));
    graphicObject.setAngle(0.0);
    graphicObjects.push_back(graphicObject);
    // ���������� ������������ �������
    meshId = rm.loadMesh("meshes/natures/big_tree.obj");
    graphicObject.setMesh(meshId);
    graphicObject.setColor(vec4(0.2, 0.8, 0.2, 1));
    graphicObject.setPosition(vec3(7.5, -0.75, 2.5));
    graphicObject.setAngle(0.0);
    graphicObjects.push_back(graphicObject);
    // ���������� ������������ �������
    meshId = rm.loadMesh("meshes/natures/big_tree.obj");
    graphicObject.setMesh(meshId);
    graphicObject.setColor(vec4(0.2, 0.8, 0.2, 1));
    graphicObject.setPosition(vec3(-7.5, -0.75, 2.5));
    graphicObject.setAngle(0.0);
    graphicObjects.push_back(graphicObject);
    // ���������� ������������ �������
    meshId = rm.loadMesh("meshes/vehicles/police_car.obj");
    graphicObject.setMesh(meshId);
    graphicObject.setColor(vec4(0.2, 0.2, 1.0, 1));
    graphicObject.setPosition(vec3(+4.5, -2.15, +6.5));
    graphicObject.setAngle(-115.0);
    graphicObjects.push_back(graphicObject);
    // ���������� ������������ �������
    meshId = rm.loadMesh("meshes/vehicles/police_car.obj");
    graphicObject.setMesh(meshId);
    graphicObject.setColor(vec4(0.23, 0.23, 1.0, 1));
    graphicObject.setPosition(vec3(+4.25, -2.15, +10.5));
    graphicObject.setAngle(+105.0);
    graphicObjects.push_back(graphicObject);
    // ���������� ������������ �������
    meshId = rm.loadMesh("meshes/vehicles/jeep.obj");
    graphicObject.setMesh(meshId);
    graphicObject.setColor(vec4(0.95, 0.13, 0.13, 1));
    graphicObject.setPosition(vec3(-1.25, -2.15, +9.0));
    graphicObject.setAngle(+170.0);
    graphicObjects.push_back(graphicObject);
    // ���������� ������������ �������
    meshId = rm.loadMesh("meshes/vehicles/jeep.obj");
    graphicObject.setMesh(meshId);
    graphicObject.setColor(vec4(0.7, 0.9, 0.13, 1));
    graphicObject.setPosition(vec3(-4.25, -2.15, +10.0));
    graphicObject.setAngle(+100.0);
    graphicObjects.push_back(graphicObject);
}

