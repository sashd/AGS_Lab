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

using namespace std;
using namespace glm;

void Display(void);
void Reshape(int w, int h);
void Simulation();
void drawCube();
float getSimulationTime();
void input(float);
void mouseWheel(int wheel, int direction, int x, int y);

Shader shader;
Camera camera;
float w = 800.0f, h = 600.0f; // разрешение экрана
float moveSpeed = 0.05f; // camera move speed
float scrollSpeed = 0.5f; // camera zoom speed
float mouse_sensitivity = 50; // camera rotate speed
POINT oldCursPos;
POINT newCursPos;

const int modelCount = 7;
mat4 modelMatrices[modelCount] =
{
    mat4(
        vec4(1, 0, 0, 0),
        vec4(0, 1, 0, 0),
        vec4(0, 0, 1, 0),
        vec4(0, 0, 0, 1)),

    mat4(
        vec4(1, 0, 0, 0),
        vec4(0, 1, 0, 0),
        vec4(0, 0, 1, 0),
        vec4(1.5, 0, 0, 1)),

    mat4(
        vec4(1, 0, 0, 0),
        vec4(0, 1, 0, 0),
        vec4(0, 0, 1, 0),
        vec4(-1.5, 0, 0, 1)),

    mat4(
        vec4(1, 0, 0, 0),
        vec4(0, 1, 0, 0),
        vec4(0, 0, 1, 0),
        vec4(0, 0, 1.5, 1)),

    mat4(
        vec4(1, 0, 0, 0),
        vec4(0, 1, 0, 0),
        vec4(0, 0, 1, 0),
        vec4(1.5, 0, 1.5, 1)),

    mat4(
        vec4(1, 0, 0, 0),
        vec4(0, 1, 0, 0),
        vec4(0, 0, 1, 0),
        vec4(-1.5, 0, 1.5, 1)),

    mat4(
        vec4(1, 0, 0, 0),
        vec4(0, 1, 0, 0),
        vec4(0, 0, 1, 0),
        vec4(-1.5, 1.5, 1.5, 1))  
};

vec4 modelColors[modelCount] = 
{
    vec4(1, 0, 0, 1),
    vec4(0, 1, 0.5, 1),
    vec4(0, 0, 1, 1),
    vec4(1, 1, 0, 1),
    vec4(0.5, 0.5, 0, 1),
    vec4(1, 0, 1, 1),
    vec4(0, 0, 0.2, 1)
};

void main(int argc, char** argv)
{
    // инициализация библиотеки GLUT
    glutInit(&argc, argv);

    // инициализация дисплея (формат вывода)
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);

    // требования к версии OpenGL (версия 3.3 без поддержки обратной совместимости)
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    // устанавливаем верхний левый угол окна
    glutInitWindowPosition(300, 300);

    // устанавливаем размер окна
    glutInitWindowSize(w, h);

    // создание окна
    glutCreateWindow("LAB#2");

    // инициализация GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "Glew error: %s\n", glewGetErrorString(err));
        return;
    }

    // определение текущей версии OpenGL
    printf("OpenGL Version = %s\n\n", glGetString(GL_VERSION));

    // установка позиции камеры
    camera.setPosition(vec3(10, 1, 1));

    // загрузка шейдера
    shader.load("Shaders\\Example.vsh", "Shaders\\Example.fsh");
    // устанавливаем функцию, которая будет вызываться для перерисовки окна
    glutDisplayFunc(Display);
    // устанавливаем функцию, которая будет вызываться при изменении размеров окна
    glutReshapeFunc(Reshape);
    // устанавливаем функцию которая вызывается всякий раз, когда процессор простаивает
    glutIdleFunc(Simulation);
    // функция колесика мыши
    glutMouseWheelFunc(mouseWheel);

    // основной цикл обработки сообщений ОС
    glutMainLoop();
    return;
}

void Display(void)
{
    // очищаем буфер кадра
    glClearColor(0.7, 0.7, 0.7, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // включаем тест глубины (на всякий случай)
    glEnable(GL_DEPTH_TEST);

    // некоторые дополнительные параметры рендеринга
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // активируем шейдер, используемый для вывода объекта
    shader.activate();

    // устанавливаем матрицу проекции
    mat4& projectionMatrix = camera.getProjectionMatrix();
    shader.setUniform("projectionMatrix", projectionMatrix);

    // получаем матрицу камеры
    mat4& viewMatrix = camera.getViewMatrix();

    // выводим все объекты
    for (int i = 0; i < modelCount; i++) 
    {
        // устанавливаем матрицу наблюдения модели
        mat4 modelViewMatrix = viewMatrix * modelMatrices[i];
        shader.setUniform("modelViewMatrix", modelViewMatrix);
        // устанавливаем цвет
        shader.setUniform("color", modelColors[i]);
        // выводим модель
        drawCube();
    }

    // меняем буферы
    glutSwapBuffers();
}

// функция, вызываемая при изменении размеров окна
void Reshape(int w, int h)
{
    // установить новую область просмотра, равную всей области окна
    glViewport(0, 0, w, h);
    // устанавливаем матрицу проекции
    camera.setProjectionMatrix(radians(45.0f), float(w) / h, 1.0f, 100.0f);
}

// функция вызывается когда процессор простаивает
void Simulation()
{
    input(getSimulationTime());
    glutPostRedisplay();
    glutIdleFunc(Simulation);
}

// вывод куба
void drawCube()
{
    // переменные для вывода объекта (прямоугольника из двух треугольников)
    static GLuint VAO_Index = 0; // индекс VAO-буфера
    static GLuint VBO_Index = 0; // индекс VBO-буфера
    static int VertexCount = 0; // количество вершин
    static bool init = true;
    if (init) {
        // создание и заполнение VBO
        glGenBuffers(1, &VBO_Index);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
        GLfloat Verteces[] = {
            // передняя грань (два треугольника)
            -0.5, +0.5, +0.5, -0.5, -0.5, +0.5, +0.5, +0.5, +0.5,
            +0.5, +0.5, +0.5, -0.5, -0.5, +0.5, +0.5, -0.5, +0.5,
            // задняя грань (два треугольника)
            +0.5, +0.5, -0.5, +0.5, -0.5, -0.5, -0.5, +0.5, -0.5,
            -0.5, +0.5, -0.5, +0.5, -0.5, -0.5, -0.5, -0.5, -0.5,
            // правая грань (два треугольника)
            +0.5, -0.5, +0.5, +0.5, -0.5, -0.5, +0.5, +0.5, +0.5,
            +0.5, +0.5, +0.5, +0.5, -0.5, -0.5, +0.5, +0.5, -0.5,
            // левая грань (два треугольника)
            -0.5, +0.5, +0.5, -0.5, +0.5, -0.5, -0.5, -0.5, +0.5,
            -0.5, -0.5, +0.5, -0.5, +0.5, -0.5, -0.5, -0.5, -0.5,
            // верхняя грань (два треугольника)
            -0.5, +0.5, -0.5, -0.5, +0.5, +0.5, +0.5, +0.5, -0.5,
            +0.5, +0.5, -0.5, -0.5, +0.5, +0.5, +0.5, +0.5, +0.5,
            // нижняя грань (два треугольника)
            -0.5, -0.5, +0.5, -0.5, -0.5, -0.5, +0.5, -0.5, +0.5,
            +0.5, -0.5, +0.5, -0.5, -0.5, -0.5, +0.5, -0.5, -0.5
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(Verteces), Verteces, GL_STATIC_DRAW);
        // создание VAO
        glGenVertexArrays(1, &VAO_Index);
        glBindVertexArray(VAO_Index);
        // инициализация VAO
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
        int location = 0;
        glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(location);

        // "отвязка" буфера VAO
        glBindVertexArray(0);

        // указание количество вершин
        VertexCount = 6 * 6;
        init = false;
    }
    // вывод модели на экран
    glBindVertexArray(VAO_Index);
    glDrawArrays(GL_TRIANGLES, 0, VertexCount);
}

// Получить deltaTime
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

// функция обработки вращения колесика мышки
void mouseWheel(int wheel, int direction, int x, int y)
{
    // определяем, на сколько необходимо приблизить/удалить камеру
    float delta = -direction * scrollSpeed;
    camera.zoom(delta);
}

