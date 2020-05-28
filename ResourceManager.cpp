#include "ResourceManager.h"

using namespace std;

int ResourceManager::loadMesh(string filename)
{
    // ���� ��� � ���������
    auto id = meshes_id.find(filename);
    if (id != meshes_id.end())
    {
        // ��������� ���, ������� ��� ����
        cout << "Loaded existing mesh" << endl;
        return id->second;
    }
    // ���� ��� �� ������, ��������� � ���������
    Mesh mesh;
    if (mesh.load(filename))
    {
        meshes.push_back(mesh);
        int id = meshes.size() - 1;
        meshes_id[filename] = id;
        return id;
    }

    return -1;
}

Mesh* ResourceManager::getMesh(int index)
{
    if (index > meshes.size() - 1)
    {
        return NULL;
    }

    return &meshes[index];
}

