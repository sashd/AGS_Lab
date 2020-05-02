#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include <glew.h>
#include <GL/freeglut.h> 
#include <GL/glut.h>
#include <GL/gl.h> 
#include "glm/glm.hpp"
#include <map>

class Shader
{
public:
    // Загрузка шейдера из внешних файлов
    bool load(std::string vertexShaderFilename, std::string fragmentShaderFilename);

    // Активировать шейдерную программу
    void activate();

    // Деактивировать шейдерную программу
    void deactivate();

    // Установка uniform-переменных
    void setUniform(std::string name, glm::vec4& value);

    // Установка uniform-переменных
    void setUniform(std::string name, glm::mat4& value);

private:
    // шейдерная программа (шейдер)
    GLuint program;
    // шейдерный объект
    GLuint shader;
    std::map<std::string, GLuint>uniforms; // хранит имя переменной и ее location
    // Создание шейдерного объекта и загрузка текста шейдера из файла
    GLuint createShaderObject(GLenum shaderType, std::string filename);
    // получение индекса (location) uniform-переменной
    GLuint getUniformLocation(std::string name);
};

