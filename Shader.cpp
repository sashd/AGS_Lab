#include "Shader.h"

using namespace std;
using namespace glm;

bool Shader::load(string vertexShaderFilename, string fragmentShaderFilename)
{
    // Cоздаем шейдерную программу
    program = glCreateProgram();


    // фрагментный шейдерный объект
    shader = createShaderObject(GL_FRAGMENT_SHADER, fragmentShaderFilename);
    glAttachShader(program, shader);

    // вершинный шейдерный объект
    shader = createShaderObject(GL_VERTEX_SHADER, vertexShaderFilename);
    glAttachShader(program, shader);

    // Линковка
    glLinkProgram(program);
    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE)
    {
        char errorMessage[300];
        GLsizei errorMsgLen;
        glGetProgramInfoLog(program, 300, &errorMsgLen, errorMessage);
        cout << errorMessage << endl;
        return false;
    }
    else 
    {
        cout << "Link process successful" << endl;
    }
}

GLuint Shader::createShaderObject(GLenum shaderType, std::string filename)
{
    GLuint sh = glCreateShader(shaderType);
    // подразумевается, что в шейдерной программе не более 10000 знаков
    char source[10000];

    ifstream f(filename);
    if (!f.is_open())
    {
        cout << "Can't open file " << filename << endl;
        return false;
    }

    f.getline(source, 10000, 0);
    // передача текста шейдера в шейдерный объект
    char* src[1] = { source };

    glShaderSource(sh, 1, (const char**)src, NULL);
    glCompileShader(sh);
    GLint compileStatus;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus != GL_TRUE)
    {
        char errorMessage[300];
        GLsizei errorMsgLen;
        glGetShaderInfoLog(sh, 300, &errorMsgLen, errorMessage);
        cout << errorMessage << endl;
        return false;
    }
    else 
    {
        return sh;
    }
}

void Shader::activate()
{
    glUseProgram(program);
}

void Shader::deactivate()
{
    glUseProgram(0);
}

void Shader::setUniform(string name, vec4 value)
{
    GLint location = getUniformLocation(name);
    glUniform4f(location, value.r, value.g, value.b, value.a);
}

void Shader::setUniform(string name, mat4 value)
{
    GLint location = getUniformLocation(name);
    glUniformMatrix4fv(uniforms[name], 1, GL_FALSE, value_ptr(value));

}

void Shader::setUniform(string name, float value)
{
    uniforms[name] = getUniformLocation(name);
    glUniform1f(uniforms[name], value);
}

void Shader::setUniform(string name, vec2 value)
{
    uniforms[name] = getUniformLocation(name);
    glUniformMatrix2fv(uniforms[name], 1, GL_FALSE, value_ptr(value));
}

GLuint Shader::getUniformLocation(string name) 
{
    auto it = uniforms.find(name);
    if (it != uniforms.end())
    {
        return it->second;
    }
    else 
    {
        GLint loc = glGetUniformLocation(program, name.c_str());
        uniforms.insert(make_pair(name, loc));
        return loc;
    }
}