#include "GraphicObject.h"

using namespace glm;
using namespace std;

GraphicObject::GraphicObject()
{
    meshId = -1;
    position = vec3(0, 0, 0);
    angle = 0;
    color = vec4(1, 1, 1, 1);
    updModelMatrix();
}

void GraphicObject::setMesh(int meshId)
{
    this->meshId = meshId;
}

void GraphicObject::setColor(const vec4& color)
{
    this->color = color;
}

void GraphicObject::setPosition(const vec3& position)
{
    this->position = position;
    updModelMatrix();
}

void GraphicObject::setAngle(float angle)
{
    this->angle = angle;
    updModelMatrix();
}

int GraphicObject::getMeshId()
{
    return meshId;
}

vec4& GraphicObject::getColor()
{
    return color;
}

mat4& GraphicObject::getModelMatrix()
{
    return modelMatrix;
}

void GraphicObject::setMaterial(Material& material)
{
    this->material = material;
}

Material& GraphicObject::getMaterial()
{
    return material;
}

void GraphicObject::updModelMatrix() 
{
    modelMatrix = translate(this->position) * rotate(radians(this->angle), vec3(0.0f, 1.0f, 0.0f));
}


