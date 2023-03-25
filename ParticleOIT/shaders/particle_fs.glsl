#version 450 core
#extension GL_EXT_texture_array:enable
#include "Common.glsl"
flat in uint particleType;
uniform sampler2DArray uSmokeTexture;
layout(std430, binding = STORAGE_RENDER_PARAMETERS)
readonly buffer RenderParameters {
  SRenderParameters renderParameters[];
};

layout(location = 0) out vec4 fragColor;


void main() { 
  fragColor = texture2DArray(uSmokeTexture,vec3(gl_PointCoord, particleType));
  if(fragColor.a < 0.01f) discard;
  fragColor.a *= renderParameters[particleType].alpha_factor;
  fragColor.rgb = fragColor.rgb * renderParameters[particleType].particle_color.rgb;
}

