#version 450 core

#include "Common.glsl"

layout(location=0) in vec3 position;
layout(location=1) in vec3 velocity;  //unused
layout(location=2) in vec2 age_info;
layout(location=3) in uint type;

layout(std140,binding=0) uniform u_Matrices4ProjectionWorld
{
    mat4 u_ProjectionMatrix;
    mat4 u_ViewMatrix;
};



uniform vec3 uBirthGradient = vec3(1.0f, 0.0f, 0.0f);
uniform vec3 uDeathGradient = vec3(0.0f);

 
layout(std430, binding = STORAGE_RENDER_PARAMETERS)
readonly buffer RenderParameters {
  SRenderParameters renderParameters[];
};

 

// ----------------------------------------------------------------------------

/* Map a range from [edge0, edge1] to [0, 1]. */
float maprange(float edge0, float edge1, float x) {
  return clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
}

// ----------------------------------------------------------------------------

/* Map a value in [0, 1] to peak at edge. */
float curve_inout(in float x, in float edge) {
  // Coefficient for sub range.
  float a = maprange(0.0f, edge, x);
  float b = maprange(edge, 1.0f, x);

  // Quadratic ease-in / quadratic ease-out.
  float easein = a * (2.0f - a);        // a * a;
  float easeout = b*b - 2.0f*b + 1.0f;  // 1.0f - b * b;

  // chose between easin / easout function.
  float result = mix(easein, easeout, step(edge, x));

  // Makes particles fade-in and out of existence
  return result;
}

// ----------------------------------------------------------------------------

float compute_size(float z, float decay) {
  const float min_size = renderParameters[type].min_size;
  const float max_size = renderParameters[type].max_size;

  // tricks to 'zoom-in' the pointsprite, just set to 1 to have normal size.
  const float depth = (max_size-min_size) / (z);

  float size = mix(min_size, max_size, decay * depth);

  return size;
}
 
 flat out uint particleType;
void main() {

if(type==0u||type==1u)
{
  const vec3 p = position.xyz;

  // Time alived in [0, 1].
  const float dAge = 1.0f - maprange(0.0f, age_info.x, age_info.y);
  const float decay = curve_inout(dAge, 0.55f);

  // Vertex attributes.
  gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(p, 1.0f);
  gl_PointSize = compute_size(gl_Position.z, decay) * renderParameters[type].size_scale_factor;

  particleType = type;
}


}

// ----------------------------------------------------------------------------
