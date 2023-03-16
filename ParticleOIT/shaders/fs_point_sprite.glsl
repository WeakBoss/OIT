#version 450 core

// ----------------------------------------------------------------------------

#include "inc_rendering_shared.glsl"

// ----------------------------------------------------------------------------

in VDataBlock {
  vec3 position; // unused legacy of vs_generic
  vec3 velocity; // idem
  vec3 color;
  float decay;
  float pointSize;
} IN;

layout(location = 0) out vec4 fragColor;

// ----------------------------------------------------------------------------

void main() { 
  fragColor = vec4(1.0f,1.0f,1.0f,1.0f);
}

// ----------------------------------------------------------------------------
