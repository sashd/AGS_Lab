#pragma once
#include "Camera.h"
#include "GraphicObject.h"
#include "Light.h"
#include "Material.h"
#include "Shader.h"
#include "ResourceManager.h"
#include <ctime>

// ����� ��� �������������� � OPENGL
// ���� ����� �������������� ����� ������������ ��������� ������� ������
class RenderManager
{
public:
    // ������-����� ��� ��������� ���������� ������
    static RenderManager& instance()
    {
        static RenderManager renderManager;
        return renderManager;
    }
    // ������������� ������� RenderManager, ����������� ����� ������������� OpenGL:
    // �������� ��������, ��������� ���������� ���������� � ������ �������������
    void init();
    // ������ ������ ���������� ����� (����������, ������� ������� ����������� ��������)
    void start();
    // ��������� ������������ ������
    void setCamera(Camera* camera);
    // ��������� ������������� ��������� �����
    void setLight(Light* light);
    // ���������� � ������� ���������� ���������� ������� ��� ������
    void addToRenderQueue(GraphicObject* graphicObject);
    // ���������� ������ ����� (�������� ������)
    void finish();
    // ��������� ���������� FPS
    float getFPS();
private:
    // ����������� �� ��������� (���������)
    RenderManager() {};
    // ������������ ����������� ���
    RenderManager(const RenderManager& root) = delete;
    // ��������� ������������ ���
    RenderManager& operator=(const RenderManager&) = delete;
    // ������ ���������� FPS
    void calcFPS();
private:
    // ������������ �������
    std::vector<Shader> shaders;
    // ��������� �� ������
    Camera* camera;
    // ��������� �� �������� �����
    Light* light;
    // ������������ ������
    Shader shader;

    // ��������� �� ��� ����������� �������, ������� ���������� ������� � ������ �����
    std::vector<GraphicObject*> graphicObjects;
    // ���������� ���������� ������ � �������
    float FPS;
    std::clock_t startTime;
    std::clock_t elapsedTime;
    int frames;
};