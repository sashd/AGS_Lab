#include "RenderManager.h"
#include <iostream>

using namespace std;
using namespace glm;

// инициализация объекта RenderManager, выполняется после инициализации OpenGL:
// загрузка шейдеров, установка неизменных параметров и прочая инициализация
void RenderManager::init()
{
    glClearColor(0.99, 0.99, 0.99, 1);

    // загрузка шейдера
    shader.load("Shaders\\PhongLighting.vsh", "Shaders\\PhongLighting.fsh");
    shader.activate();

}

// Начало вывода очередного кадра (подготовка, очистка вектора графических объектов)
void RenderManager::start()
{
    graphicObjects.clear();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

// добавление в очередь рендеринга очередного объекта для вывода
void RenderManager::addToRenderQueue(GraphicObject* graphicObject)
{
    graphicObjects.push_back(graphicObject);
}

// завершение вывода кадра (основная работа)
void RenderManager::finish()
{
    mat4& viewMatrix = camera->getViewMatrix();

    shader.setUniform("projectionMatrix", camera->getProjectionMatrix());
    shader.setUniform("lAmbient", light->getAmbient());
    shader.setUniform("lDiffuse", light->getDiffuse());
    shader.setUniform("lSpecular", light->getSpecular());
    shader.setUniform("lPosition", viewMatrix * light->getDirection());

    // выводим все объекты
    for (int i = 0; i < graphicObjects.size(); i++) 
    {
        // устанавливаем матрицу наблюдения модели
        mat4 modelViewMatrix = viewMatrix * graphicObjects[i]->getModelMatrix();
        shader.setUniform("modelViewMatrix", modelViewMatrix);

        // устанавливаем параметры материала
        Material& material = graphicObjects[i]->getMaterial();
        shader.setUniform("mAmbient", material.getAmbient());
        shader.setUniform("mDiffuse", material.getDiffuse());
        shader.setUniform("mSpecular", material.getSpecular());
        shader.setUniform("mShininess", material.getShininess());

        // выводим меш
        int meshId = graphicObjects[i]->getMeshId();
        Mesh* mesh = ResourceManager::instance().getMesh(meshId);
        if (mesh != nullptr)
            mesh->draw();
    }
    calcFPS();
}

void RenderManager::setCamera(Camera* camera)
{
    this->camera = camera;
}

void RenderManager::setLight(Light* light)
{
    this->light = light;
}

void RenderManager::calcFPS()
{
    elapsedTime += clock() - startTime;
    startTime = clock();
    frames++;
    if (elapsedTime / CLOCKS_PER_SEC > 1)
    {
        FPS = frames / (elapsedTime / CLOCKS_PER_SEC);
        elapsedTime = 0;
        frames = 0;
    }
}

float RenderManager::getFPS()
{
    return FPS;
}

