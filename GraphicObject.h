#pragma once
#include "Mesh.h"

// КЛАСС ДЛЯ РАБОТЫ С ГРАФИЧЕСКИМ ОБЪЕКТОМ
class GraphicObject
{
public:
    // конструктор по умолчанию
    GraphicObject();

    // установить используемый меш (идентификатор)
    void setMesh(int meshId);

    // установить цвет объекта
    void setColor(const glm::vec4& color);

    // установить позицию объекта
    void setPosition(const glm::vec3& position);

    // установить угол поворота в градусах относительно оси OY по часовой стрелке
    void setAngle(float angle);

    // получить различные параметры
    int getMeshId();

    glm::vec4& getColor();
    glm::mat4& getModelMatrix();

private:
    // используемый меш (индекс в менеджере ресурсов)
    int meshId;

    // положение
    glm::vec3 position;

    // угол поворота
    float angle;

    // цвет объекта
    glm::vec4 color;

    // матрица модели (задает позицию и ориентацию)
    glm::mat4 modelMatrix;

    // установка матрицы модели
    void updModelMatrix();
};
