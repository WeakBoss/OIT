#version 430 core

// ============================================================================

/* Second Stage of the particle system :
 * - Calculate forces (eg. curl noise, vector field, ..),
 * - Performs time integration,
 * - Handle collision detection,
 * - Update particle position and velocity,
 */

// ============================================================================

#include "Common.glsl"
#include "inc_curlnoise.glsl"

// ----------------------------------------------------------------------------

// Time integration step.
uniform float uTimeStep;

// Vector field sampler.
uniform sampler3D uVectorFieldSampler;


float uScatteringFactor;
float uVectorFieldFactor;
float uCurlNoiseFactor;
float uCurlNoiseScale;
float uVelocityFactor;

uint uEnableScattering;
uint uEnableVectorField;
uint uEnableCurlNoise;
uint uEnableVelocityControl;

// ----------------------------------------------------------------------------

layout(binding = ATOMIC_COUNTER_BINDING_FIRST)
uniform atomic_uint read_count;

layout(binding = ATOMIC_COUNTER_BINDING_SECOND)
uniform atomic_uint write_count;

// ----------------------------------------------------------------------------

#if SPARKLE_USE_SOA_LAYOUT

// READ
layout(std430, binding = STORAGE_BINDING_PARTICLE_POSITIONS_A)
readonly buffer PositionBufferA {
  vec4 read_positions[];
};
layout(std430, binding = STORAGE_BINDING_PARTICLE_VELOCITIES_A)
readonly buffer VelocityBufferA {
  vec4 read_velocities[];
};
layout(std430, binding = STORAGE_BINDING_PARTICLE_ATTRIBUTES_A)
readonly buffer AttributeBufferA {
  vec4 read_attributes[];
};

// WRITE
layout(std430, binding = STORAGE_BINDING_PARTICLE_POSITIONS_B)
writeonly buffer PositionBufferB {
  vec4 write_positions[];
};
layout(std430, binding = STORAGE_BINDING_PARTICLE_VELOCITIES_B)
writeonly buffer VelocityBufferB {
  vec4 write_velocities[];
};
layout(std430, binding = STORAGE_BINDING_PARTICLE_ATTRIBUTES_B)
writeonly buffer AttributeBufferB {
  vec4 write_attributes[];
};

#else

// READ
layout(std430, binding = STORAGE_BINDING_PARTICLES_FIRST)
readonly buffer ParticleBufferA {
  TParticle read_particles[];
};

// WRITE
layout(std430, binding = STORAGE_BINDING_PARTICLES_SECOND)
writeonly buffer ParticleBufferB {
  TParticle write_particles[];
};

#endif  // SPARKLE_USE_SOA_LAYOUT

layout(std430, binding = STORAGE_BINDING_RANDOM_VALUES)
readonly buffer RandomBuffer {
  float randbuffer[];
};

layout(std140, binding = SIMULATE_PARAMETER_UNIFORM)
uniform SimulationParameters{
   SSimulationParameters simulationParameters[2];
};

// ----------------------------------------------------------------------------

TParticle PopParticle() {
  const uint index = gl_GlobalInvocationID.x;
  atomicCounterDecrement(read_count);

  TParticle p;

#if SPARKLE_USE_SOA_LAYOUT
  p.position   = read_positions[index];
  p.velocity   = read_velocities[index];
  vec4 attribs = read_attributes[index];

  p.start_age  = attribs.x;
  p.age        = attribs.y;
  p.id         = floatBitsToUint(attribs.w);
#else
  p = read_particles[index];
#endif

  return p;
}

void PushParticle(in TParticle p) {
  const uint index = atomicCounterIncrement(write_count);

#if SPARKLE_USE_SOA_LAYOUT
  write_positions[index]  = p.position;
  write_velocities[index] = p.velocity;
  write_attributes[index] = vec4(p.start_age, p.age, 0.0f, uintBitsToFloat(p.id));
#else
  write_particles[index] = p;
#endif
}

// ----------------------------------------------------------------------------

float GetUpdatedAge(in const TParticle p) {
  return clamp(p.age - uTimeStep, 0.0f, p.start_age);
}

// ----------------------------------------------------------------------------

void UpdateParticle(inout TParticle p,
                    in vec3 pos,
                    in vec3 vel,
                    in float age) {
  p.position.xyz = pos;
  p.velocity.xyz = vel;
  p.age = age;
}

// ----------------------------------------------------------------------------

vec3 CalculateScattering() {
  if (uEnableScattering == 0u) {
    return vec3(0.0f);
  }
  const uint gid = gl_GlobalInvocationID.x;
  vec3 randforce = vec3(randbuffer[gid], randbuffer[gid+1u], randbuffer[gid+2u]);
       randforce = 2.0f * randforce - 1.0f;
  return uScatteringFactor * randforce;
}

// ----------------------------------------------------------------------------

vec3 CalculateRepulsion(in const TParticle p) {
  vec3 push = vec3(0.0f);

  /*
  // IDEA
  const vec3 vel = p.velocity.xyz;
  const vec3 pos = p.position.xyz;
  const float MAX_INFLUENCE_DISTANCE = 8.0f;

  vec3 n;
  float d = compute_gradient(pos, n);
  float coeff = smoothstep(0.0f, MAX_INFLUENCE_DISTANCE, abs(d));
  push = coeff * (n);
  //vec3 side = cross(cross(n, normalize(vel + vec3(1e-5))), n);
  //push = mix(push, side, coeff*coeff);
  */

  return push;
}

// ----------------------------------------------------------------------------

vec3 CalculateTargetMesh(in const TParticle p) {
  vec3 pull = vec3(0.0f);

  /*
  // IDEA
  vec3 anchor = anchors_buffer[p.anchor_id];
  mat4x4 anchorModel = achor_models_buffer[p.anchor_id];
  vec3 target = anchorModel * vec4(anchor, 1.0f);
  vec3 pull = target - p.position;
  float length_pull = length(pull);
  pull *= inversesqrt(length_pull));
  float factor = MAX_PULL_FACTOR * smoothstep(0.0f, MAX_PULL_FACTOR, length_pull);
  pull *= factor;
  */

  return pull;
}

// ----------------------------------------------------------------------------

vec3 CalculateVectorField(in const TParticle p) {
  if (uEnableVectorField == 0u) {
    return vec3(0.0f);
  }

  const vec3 extent = 0.5f * vec3(textureSize(uVectorFieldSampler, 0).xyz);
  const vec3 texcoord = (p.position.xyz + extent) / (2.0f * extent);
  vec3 vfield = texture(uVectorFieldSampler, texcoord).xyz;

  return uVectorFieldFactor * vfield;
}

// ----------------------------------------------------------------------------

vec3 CalculateCurlNoise(in const TParticle p) {
  if (uEnableCurlNoise == 0u) {
    return vec3(0.0f);
  }
  vec3 curl_velocity = compute_curl(p.position.xyz * uCurlNoiseScale);
  return uCurlNoiseFactor * curl_velocity;
}


// ----------------------------------------------------------------------------

vec3 CalculateForces(in const TParticle p) {
  vec3 force = vec3(0.0f);

  force += CalculateScattering();
  force += CalculateRepulsion(p);
  force += CalculateTargetMesh(p);
  force += CalculateVectorField(p);
  force += CalculateCurlNoise(p);

  return force;
}


// ----------------------------------------------------------------------------

layout(local_size_x = PARTICLE_SYSTEM_KERNEL_GROUP_WIDTH) in;
void main() {
  // Local copy of the particle.
   TParticle p = PopParticle();

   uint Type = p.type;

    uScatteringFactor = simulationParameters[Type].scattering_factor;
    uVectorFieldFactor = simulationParameters[Type].vectorfield_factor;;
    uCurlNoiseFactor = simulationParameters[Type].curlnoise_factor;
    uCurlNoiseScale = simulationParameters[Type].curlnoise_scale;
    uVelocityFactor = simulationParameters[Type].vectorfield_factor;

    uEnableScattering = simulationParameters[Type].enable_scattering;
    uEnableVectorField = simulationParameters[Type].enable_vectorfield;
    uEnableCurlNoise = simulationParameters[Type].enable_curlnoise;
    uEnableVelocityControl = simulationParameters[Type].enable_velocity_control;

  float age = GetUpdatedAge(p);

  if (age > 0.0f) {
    // Calculate external forces.
    vec3 force = CalculateForces(p);

    // Integrations vectors.
    const vec3 dt = vec3(uTimeStep);
    vec3 velocity = p.velocity.xyz;
    vec3 position = p.position.xyz;

    // Integrate velocity.
    velocity = fma(force, dt, velocity);

    if (uEnableVelocityControl == 1u) {
      velocity = uVelocityFactor * normalize(velocity);
    }

    // Integrate position.
    position = fma(velocity, dt, position);

    // Update the particle.
    UpdateParticle(p, position, velocity, age);

    // Save it in buffer.
    PushParticle(p);
  }
}
