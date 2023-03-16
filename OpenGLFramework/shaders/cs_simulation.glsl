#version 450 core

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
uniform uint  uNumAliveParticles;

// Vector field sampler.
uniform sampler3D uVectorFieldSampler;


float uScatteringFactor;
float uVectorFieldFactor;
float uCurlNoiseFactor;
float uCurlNoiseScale;
float uVelocityFactor;
float uTimeStepFactor;

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
 

layout(std430, binding = STORAGE_BINDING_RANDOM_VALUES)
readonly buffer RandomBuffer {
  float randbuffer[];
};

layout(std430, binding = STORAGE_SIMULATE_PARAMETER)
readonly buffer SimulationParameters{
   SSimulationParameters simulationParameters[];
};

// ----------------------------------------------------------------------------


void PushParticle(in TParticle p) {
  const uint index = atomicCounterIncrement(write_count);

 
  write_particles[index] = p;
 
}

// ----------------------------------------------------------------------------

float GetUpdatedAge(in const TParticle p) {
  return clamp(p.age - uTimeStep * uTimeStepFactor, 0.0f, p.start_age);
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

   const uint index = gl_GlobalInvocationID.x;

   if(index < uNumAliveParticles)
   {
        atomicCounterDecrement(read_count);
        TParticle p = read_particles[index];

        uint Type = p.type;

        uScatteringFactor = simulationParameters[Type].scattering_factor;
        uVectorFieldFactor = simulationParameters[Type].vectorfield_factor;;
        uCurlNoiseFactor = simulationParameters[Type].curlnoise_factor;
        uCurlNoiseScale = simulationParameters[Type].curlnoise_scale;
        uVelocityFactor = simulationParameters[Type].vectorfield_factor;
        uTimeStepFactor = simulationParameters[Type].time_step_factor;

        uEnableScattering = simulationParameters[Type].enable_scattering;
        uEnableVectorField = simulationParameters[Type].enable_vectorfield;
        uEnableCurlNoise = simulationParameters[Type].enable_curlnoise;
        uEnableVelocityControl = simulationParameters[Type].enable_velocity_control;

        float age = GetUpdatedAge(p);

        if (age > 0.0f) {
        // Calculate external forces.
        vec3 force = CalculateForces(p);

        // Integrations vectors.
        const vec3 dt = vec3(uTimeStep) * uTimeStepFactor;
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
}
