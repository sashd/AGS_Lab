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
    // очищаем буфер кадра
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // включаем тест глубины (на всякий случай)
    glEnable(GL_DEPTH_TEST);
    // активируем шейдер
    shader.activate();

    // устанавливаем значение uniform-переменной для цвета
    vec4 color1 = vec4(0.0, 0.0, 1.0, 1.0);
    shader.setUniform("color1", color1);
    vec4 color2 = vec4(1.0, 0.0, 0.0, 1.0);
    shader.setUniform("color2", color2);

    shader.setUniform("offset", offset);

    // выводим прямоугольник
    DrawObject();
    // смена переднего и заднего буферов
    glutSwapBuffers();
};
// функция, вызываемая при изменении размеров окна
void Reshape(int w, int h)
{
    // установить новую область просмотра, равную всей области окна
    glViewport(0, 0, w, h);
};


// функция вызывается когда процессор простаивает
void Simulation()
{
    input(getSimulationTime());
    glutPostRedisplay();
    glutIdleFunc(Simulation);
}

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
    glutInitWindowPosition(300, 100);
    // устанавливаем размер окна
    glutInitWindowSize(800, 600);
    // создание окна
    glutCreateWindow("laba_01");
    // инициализация GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "Glew error: %s\n", glewGetErrorString(err));
        return;
    }
    // определение текущей версии OpenGL
    printf("OpenGL Version = %s\n\n", glGetString(GL_VERSION));
    // загрузка шейдера
    shader.load("Shaders\\Example.vsh", "Shaders\\Example.fsh");
    // устанавливаем функцию, которая будет вызываться для перерисовки окна
    glutDisplayFunc(Display);
    // устанавливаем функцию, которая будет вызываться при изменении размеров окна
    glutReshapeFunc(Reshape);
    // устанавливаем функцию которая вызывается всякий раз, когда процессор простаивает
    glutIdleFunc(Simulation);

    // основной цикл обработки сообщений ОС
    glutMainLoop();
    return;
}
// Функция для вывода прямоугольника
void DrawObject() {
    // переменные для вывода объекта (прямоугольника из двух треугольников)
    static bool init = true;
    static GLuint VAO_Index = 0; // индекс VAO-буфера
    static GLuint VBO_Index = 0; // индекс VBO-буфера
    static int VertexCount = 0; // количество вершин
    // при первом вызове инициализируем VBO и VAO
    if (init) {
        init = false;
        // создание и заполнение VBO
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
        // создание VAO
        glGenVertexArrays(1, &VAO_Index);
        glBindVertexArray(VAO_Index);
        // заполнение VAO
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
        // количество вершин
        VertexCount = 6;
    }
    // выводим прямоугольник
    glBindVertexArray(VAO_Index);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    return;
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

// управление прямоугольником с помощью WASD
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
