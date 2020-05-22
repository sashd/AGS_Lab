#pragma once
#include "Mesh.h"

// ����� ��� ������ � ����������� ��������
class GraphicObject
{
public:
    // ����������� �� ���������
    GraphicObject();

    // ���������� ������������ ��� (�������������)
    void setMesh(int meshId);

    // ���������� ���� �������
    void setColor(const glm::vec4& color);

    // ���������� ������� �������
    void setPosition(const glm::vec3& position);

    // ���������� ���� �������� � �������� ������������ ��� OY �� ������� �������
    void setAngle(float angle);

    // �������� ��������� ���������
    int getMeshId();

    glm::vec4& getColor();
    glm::mat4& getModelMatrix();

private:
    // ������������ ��� (������ � ��������� ��������)
    int meshId;

    // ���������
    glm::vec3 position;

    // ���� ��������
    float angle;

    // ���� �������
    glm::vec4 color;

    // ������� ������ (������ ������� � ����������)
    glm::mat4 modelMatrix;

    // ��������� ������� ������
    void updModelMatrix();
};
