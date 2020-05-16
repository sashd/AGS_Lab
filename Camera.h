#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/polar_coordinates.hpp"
#include "glm/gtx/euler_angles.hpp"

// ����� ��� ������ � �������
class Camera
{
public:
    // ����������� �� ���������
    Camera();
    // ���������� ������� ��������
    void setProjectionMatrix(float fovy, float aspect, float zNear, float zFar);
    // �������� ������� ��������
    glm::mat4& getProjectionMatrix();
    // �������� ������� ����
    glm::mat4& getViewMatrix();
    // ����������� ������ � ����� ���������� � �������������� ��������� (OXZ)
    void move(float dx, float dz);
    // ��������� � �������������� � ������������ ���������
    void rotate(float horizAngle, float vertAngle);
    // ����������/������� ������ �/�� ����� ����������
    void zoom(float dR);
    // ��������� ������� ������
    void setPosition(glm::vec3 position);

private:
    // ��������������� ������
    void refresh();

    // ��������� ���� ������
    glm::mat4 Mview;
    glm::mat4 Mprojection;
    glm::vec3 eyePos;
    glm::vec3 spherical;	// ����������� ����������
    glm::vec3 lookAtPoint;
};