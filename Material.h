#pragma once
#include "Mesh.h"

// ����� ��� ������ � ����������
class Material
{
public:
    // ����������� �� ���������
    Material();
    // ������� ��������� ���������� ��������� �����
    void setAmbient(glm::vec4& ambient);
    void setDiffuse(glm::vec4& diffuse);
    void setSpecular(glm::vec4& specular);
    void setShininess(float shininess);
    // ��������� ��������� ���������� ��������� �����
    glm::vec4& getAmbient();
    glm::vec4& getDiffuse();
    glm::vec4& getSpecular();
    float getShininess();
    // ��������� � ��������� id-�������� � ��������� ��������
    // (� ������ ������������ ������ �� ������������ - �� �������)
    void setTextureId(int textureId);
    int getTextureId();
private:
    // id-�������� � ��������� ��������
    int textureId;
    // ������� ������������
    glm::vec4 ambient;
    // ��������� ������������
    glm::vec4 diffuse;
    // ���������� ������������
    glm::vec4 specular;
    // ������� ����������������
    GLfloat shininess;
};