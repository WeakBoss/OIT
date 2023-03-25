#version 450 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTextureCord;

layout(std140,binding=0) uniform u_Matrices4ProjectionWorld
{
    mat4 u_ProjectionMatrix;
    mat4 u_ViewMatrix;
};
uniform mat4 u_Model;

out vec2 textureCord;
void main() {
  gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_Model * vec4(inPosition, 1.0f);
  textureCord = inTextureCord;
}

 