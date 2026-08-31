#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTex;
layout(location = 3) in vec3 aNormal;

out vec3 color;
out vec2 textureCoords;
out vec3 normal;
out vec3 currentPosition;

uniform mat4 cameraMatrix;
uniform mat4 modelMatrix;

void main()
{
    currentPosition = vec3(modelMatrix * vec4(aPos, 1.0f));
    gl_Position = cameraMatrix * vec4(currentPosition, 1.0);
    color = aColor;
    textureCoords = aTex;
    normal = aNormal;
}
