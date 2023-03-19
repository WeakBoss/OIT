#version 450 core
uniform sampler2D uSmokeTexture;
layout(location = 0) out vec4 fragColor;

void main() { 
  fragColor = texture2D(uSmokeTexture,gl_PointCoord);
  //fragColor = vec4(1.0f,1.0f,1.0f,1.0f);
}

