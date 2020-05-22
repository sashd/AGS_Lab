#include "Mesh.h"

using namespace std;
using namespace glm;

Mesh::Mesh()
{
}

Mesh::Mesh(string filename)
{
    this->load(filename);
}


bool Mesh::load(string filename)
{
    // открыть файл
    ifstream file;
    file.open(filename);
    if (!file.is_open())
    {
        cout << "File does not exist! " << filename << endl;
        return false;
    }

    vector<vec3> coordinates;
    vector<vec2> texCoords;
    vector<vec3> normals;
    vector<vec3> faces;
    char sTemp[512];
    while (!file.eof())
    {
        char ch;
        string inpStr;
        file >> inpStr;

        if (inpStr == "v")
        {
            vec3 coords;
            file >> coords.x;
            file >> coords.y;
            file >> coords.z;
            coordinates.push_back(coords);
        }

        if (inpStr == "vt")
        {
            vec2 tex_coordinates;
            file >> tex_coordinates.s;
            file >> tex_coordinates.t;
            texCoords.push_back(tex_coordinates);
        }

        if (inpStr == "vn")
        {
            vec3 normal_coords;
            file >> normal_coords.x;
            file >> normal_coords.y;
            file >> normal_coords.z;

            normals.push_back(normal_coords);
        }

        if (inpStr == "f")
        {
            for (int i = 0; i < 3; i++)
            {
                vec3 face;
                file >> face.x;
                file >> ch;
                file >> face.y;
                file >> ch;
                file >> face.z;
                faces.push_back(face);
            }
        }
        file.getline(sTemp, 512);
    }
    file.close();

    vector<Vertex> vertices;
    vector<GLuint> vIndices;
    map<string, int> m;

    int vcount = 0;

    for (auto it : faces)
    {
        vec3 vCoord = coordinates[it.x - 1];
        vec2 vTexCoord = texCoords[it.y - 1];
        vec3 vNormal = normals[it.z - 1];
        string str = to_string(it.x) + '/' + to_string(it.y) + '/' + to_string(it.z);

        auto v = m.find(str);
        if (v == m.end())
        {
            Vertex tmp{ {vCoord.x, vCoord.y, vCoord.z}, {vNormal.x, vNormal.y, vNormal.z}, {vTexCoord.s, vTexCoord.t} };
            vertices.push_back(tmp);
            m.emplace(str, vcount++);
        }
        vIndices.push_back(m[str]);
    }
    vertexCount = vIndices.size();

    glGenBuffers(2, buffers);

    // привязка VBO
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, vcount * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // привязка VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::coord));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::texCoord));
    glEnableVertexAttribArray(2);

    // привязка EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vIndices.size() * sizeof(GLuint), vIndices.data(), GL_STATIC_DRAW);

    // отвязка всех объектов
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return true;
}

void Mesh::draw()
{
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);

    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::~Mesh()
{
    glDeleteBuffers(2, buffers);
}