#version 450 core

layout(location = 0) out vec4 fragColor;
uniform sampler2D uFloorTexture;
uniform float u_ScaleFator;
in vec2 textureCord;
void main() {
  fragColor = vec4(vec3(texture(uFloorTexture,textureCord * u_ScaleFator*0.3).rgb)* 0.5f ,1.0f);
}
