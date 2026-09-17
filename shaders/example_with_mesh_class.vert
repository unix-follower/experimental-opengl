#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;
layout(location = 3) in vec2 aTex;

out vec3 currentPosition;
out vec3 normal;
out vec3 color;
out vec2 textureCoords;

uniform mat4 cameraMatrix;
uniform mat4 model;

void main()
{
    // calculate current position
    currentPosition = vec3(model * vec4(aPos, 1.0f));
    normal = aNormal;
    color = aColor;
    textureCoords = aTex;
    gl_Position = cameraMatrix * vec4(currentPosition, 1.0);
}
