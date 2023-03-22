#version 450 core

// ----------------------------------------------------------------------------

layout(location = 0) in vec3 inPosition;

layout(std140,binding=0) uniform u_Matrices4ProjectionWorld
{
    mat4 u_ProjectionMatrix;
    mat4 u_ViewMatrix;
};
uniform mat4 billBoardRotation;
uniform mat4 model;

// ----------------------------------------------------------------------------

void main() {
 
  gl_Position = u_ProjectionMatrix * u_ViewMatrix * billBoardRotation * model * vec4(inPosition, 1.0f);
}

// ----------------------------------------------------------------------------
