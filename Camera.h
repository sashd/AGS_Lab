#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/polar_coordinates.hpp"
#include "glm/gtx/euler_angles.hpp"

// КЛАСС ДЛЯ РАБОТЫ С КАМЕРОЙ
class Camera
{
public:
    // конструктор по умолчанию
    Camera();
    // установить матрицу проекции
    void setProjectionMatrix(float fovy, float aspect, float zNear, float zFar);
    // получить матрицу проекции
    glm::mat4& getProjectionMatrix();
    // получить матрицу вида
    glm::mat4& getViewMatrix();
    // передвинуть камеру и точку наблюдения в горизонтальной плоскости (OXZ)
    void move(float dx, float dz);
    // повернуть в горизонтальной и вертикальной плоскости
    void rotate(float horizAngle, float vertAngle);
    // приблизить/удалить камеру к/от точки наблюдения
    void zoom(float dR);
    // установка позиции камеры
    void setPosition(glm::vec3 position);

private:
    // вспомогательные методы
    void refresh();

    // приватные поля класса
    glm::mat4 Mview;
    glm::mat4 Mprojection;
    glm::vec3 eyePos;
    glm::vec3 spherical;	// сферические координаты
    glm::vec3 lookAtPoint;
};