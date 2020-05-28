#include "Material.h"

using namespace glm;

Material::Material()
{
    textureId = -1;
    ambient = vec4(0.35, 0.35, 0.35, 1);
    diffuse = vec4(0.85, 0.85, 0.85, 1);
    specular = vec4(0.25, 0.25, 0.25, 1);
    shininess = 64.0;
}

void Material::setAmbient(vec4& ambient)
{
    this->ambient = ambient;
}

void Material::setDiffuse(vec4& diffuse)
{
    this->diffuse = diffuse;
}

void Material::setSpecular(vec4& specular)
{
    this->specular = specular;
}

void Material::setShininess(float shininess)
{
    this->shininess = shininess;
}

vec4& Material::getAmbient() 
{
    return ambient;
}

vec4& Material::getDiffuse() 
{
    return diffuse;
}

vec4& Material::getSpecular() 
{
    return specular;
}

float Material::getShininess()
{
    return shininess;
}