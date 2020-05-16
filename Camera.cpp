#include "Camera.h"

using namespace std;
using namespace glm;

Camera::Camera() 
{}

void Camera::setProjectionMatrix(float fovy, float aspect, float zNear, float zFar)
{
    Mprojection = perspective(fovy, aspect, zNear, zFar);
}

mat4& Camera::getProjectionMatrix()
{
    return Mprojection;
}

mat4& Camera::getViewMatrix()
{
    return Mview;
}

void Camera::move(float dx, float dz)
{
    vec3 lookEye = lookAtPoint - eyePos;
    lookEye.y = 0;
    vec3 forward = normalize(lookEye);
    vec3 right = cross(vec3(0, 1, 0), forward);
    vec3 dpos = forward * dx + right * dz;
    eyePos += dpos;
    lookAtPoint += dpos;
    refresh();
}

void Camera::rotate(float horizAngle, float vertAngle) 
{
    spherical.y += horizAngle;

    if (spherical.y < -360 || spherical.y > 360)
        spherical.y -= spherical.y;

    spherical.z = clamp<float>(spherical.z + vertAngle, 5.0f, 85.0f);
    refresh();
}

void Camera::zoom(float dR)
{
    spherical.r = clamp<float>(spherical.x + dR, 3.0f, 60.0f);
    refresh();
}

void Camera::setPosition(vec3 position)
{
    eyePos = position;
    float d = distance(lookAtPoint, eyePos);
    vec3 projXY = vec3(eyePos.x, eyePos.y, 0);
    vec3 projZX = vec3(eyePos.x, 0, eyePos.z);
    spherical = vec3(d, degrees((acos(dot(normalize(eyePos), normalize(projZX))))), degrees(acos(dot(normalize(projXY), normalize(projZX)))));
    refresh();
}

void Camera::refresh()
{
    mat4 R = yawPitchRoll<float>(-radians(spherical.y), -radians(spherical.z), 0);
    vec4 T = R * vec4(0, 0, spherical.x, 0);
    eyePos = lookAtPoint + vec3(T);
    Mview = lookAt(eyePos, lookAtPoint, vec3(0, 1, 0));
}



