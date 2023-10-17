#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexture;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

out vec3 Normal;
out vec3 FragPos;

void main() {
    FragPos = (modelMatrix * vec4(inPosition, 1.0)).xyz;
    Normal = mat3(transpose(inverse(modelMatrix))) * inNormal;
    gl_Position = projectionMatrix * viewMatrix * vec4(FragPos, 1.0);
}