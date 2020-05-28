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
#include "Material.h"
#include "Light.h"
#include "RenderManager.h"

using namespace std;
using namespace glm;

void Display(void);
void Reshape(int w, int h);
void Simulation();
float getSimulationTime();
void input(float);
void mouseWheel(int wheel, int direction, int x, int y);
void initGraphicObjects();

float w = 800.0f, h = 600.0f; // разрешение экрана
float moveSpeed = 0.01f; // camera move speed
float scrollSpeed = 0.5f; // camera zoom speed
float mouse_sensitivity = 100; // camera rotate speed
POINT oldCursPos;
POINT newCursPos;

RenderManager& renderManager = RenderManager::instance();
ResourceManager& resourceManager = ResourceManager::instance();

Camera camera;
Light light;

Shader shader;
vector<GraphicObject>graphicObjects;

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
    glutCreateWindow("");

    // инициализация GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "Glew error: %s\n", glewGetErrorString(err));
        return;
    }

    // определение текущей версии OpenGL
    printf("OpenGL Version = %s\n\n", glGetString(GL_VERSION));

    camera.setPosition(vec3(10, 1, 1));
    initGraphicObjects();  // загрузка всех графический объектов в вектор
    renderManager.init();  // инициализация менеджера рендера

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

// функция для построения изображения на экране (вызывается при перерисовке окна)
void Display(void)
{
    renderManager.start();

    renderManager.setCamera(&camera);
    renderManager.setLight(&light);

    // добавляем в очередь все объекты, которые необходимо вывести
    for (int i = 0; i < graphicObjects.size(); i++) 
    {
        renderManager.addToRenderQueue(&graphicObjects[i]);
    }

    // завершаем построение кадра
    renderManager.finish();

    // меняем передний и задний буферы цвета
    glutSwapBuffers();

    // выводим количество FPS
    char tempStr[128];
    sprintf_s(tempStr, "[%06.2f FPS] Lab4", renderManager.getFPS());
    glutSetWindowTitle(tempStr);
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

// вспомогательная функция для инициализации графических объектов
void initGraphicObjects()
{
    // временная переменная для хранения идентификаторов меша
    int meshId = -1;
    // временная переменная для представления графического объекта
    GraphicObject graphicObject;
    // добавление графического объекта
    meshId = resourceManager.loadMesh("meshes/buildings/house_2.obj");
    graphicObject.setMesh(meshId);
    graphicObject.setColor(vec4(0.2f, 0.2f, 0.2f, 1.0f));
    graphicObject.setPosition(vec3(0, 0, 0));
    graphicObject.setAngle(0.0);
    graphicObjects.push_back(graphicObject);
    // добавление графического объекта
    meshId = resourceManager.loadMesh("meshes/natures/big_tree.obj");
    graphicObject.setMesh(meshId);
    graphicObject.setColor(vec4(0.2, 0.8, 0.2, 1));
    graphicObject.setPosition(vec3(7.5, -0.75, 2.5));
    graphicObject.setAngle(0.0);
    graphicObjects.push_back(graphicObject);
    // добавление графического объекта
    meshId = resourceManager.loadMesh("meshes/natures/big_tree.obj");
    graphicObject.setMesh(meshId);
    graphicObject.setColor(vec4(0.2, 0.8, 0.2, 1));
    graphicObject.setPosition(vec3(-7.5, -0.75, 2.5));
    graphicObject.setAngle(0.0);
    graphicObjects.push_back(graphicObject);
    // добавление графического объекта
    meshId = resourceManager.loadMesh("meshes/vehicles/police_car.obj");
    graphicObject.setMesh(meshId);
    graphicObject.setColor(vec4(0.2, 0.2, 1.0, 1));
    graphicObject.setPosition(vec3(+4.5, -2.15, +6.5));
    graphicObject.setAngle(-115.0);
    graphicObjects.push_back(graphicObject);
    // добавление графического объекта
    meshId = resourceManager.loadMesh("meshes/vehicles/police_car.obj");
    graphicObject.setMesh(meshId);
    graphicObject.setColor(vec4(0.23, 0.23, 1.0, 1));
    graphicObject.setPosition(vec3(+4.25, -2.15, +10.5));
    graphicObject.setAngle(+105.0);
    graphicObjects.push_back(graphicObject);
    // добавление графического объекта
    meshId = resourceManager.loadMesh("meshes/vehicles/jeep.obj");
    graphicObject.setMesh(meshId);
    graphicObject.setColor(vec4(0.95, 0.13, 0.13, 1));
    graphicObject.setPosition(vec3(-1.25, -2.15, +9.0));
    graphicObject.setAngle(+170.0);
    graphicObjects.push_back(graphicObject);

}

