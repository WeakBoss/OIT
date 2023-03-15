#version 450 core

 #include "Common.glsl"
#include "inc_math.glsl"

 
//layout(location=1) uniform uint uEmitterType;
//layout(location=2) uniform vec3 uEmitterPosition;
//layout(location=3) uniform vec3 uEmitterDirection;
//layout(location=4) uniform float uEmitterRadius;
//layout(location=5) uniform float uParticleMinAge;
//layout(location=6) uniform float uParticleMaxAge;

uint EmitterType;
vec3 EmitterPosition;
vec3 EmitterDirection;
float EmitterRadius;
float ParticleMinAge;
float ParticleMaxAge;

layout(location=0) uniform uint uEmitCount;
layout(location=1) uniform uint uNumParticleType;

layout(binding = ATOMIC_COUNTER_BINDING_FIRST)
uniform atomic_uint write_count;

#if SPARKLE_USE_SOA_LAYOUT

layout(std430, binding = STORAGE_BINDING_PARTICLE_POSITIONS_A)
writeonly buffer PositionBufferA {
  vec4 positions[];
};

layout(std430, binding = STORAGE_BINDING_PARTICLE_VELOCITIES_A)
writeonly buffer VelocityBufferA {
  vec4 velocities[];
};

layout(std430, binding = STORAGE_BINDING_PARTICLE_ATTRIBUTES_A)
writeonly buffer AttributeBufferA {
  vec4 attributes[];
};

#else

layout(std430, binding = STORAGE_BINDING_PARTICLES_FIRST)
writeonly buffer ParticleBufferA {
  TParticle particles[];
};

#endif

layout(std430, binding = STORAGE_BINDING_RANDOM_VALUES)
readonly buffer RandomBuffer {
  float randbuffer[];
};

layout(std140, binding = SIMULATE_PARAMETER_UNIFORM)
uniform SimulationParameters{
   SSimulationParameters simulationParameters[2];
};

layout(std140, binding = PARTICLE_PROPORTION_UNIFORM)
uniform ParticleProportion{
   float particleProportion[2];
};

 
uint getParticleType(uint vId)
{
   const float single_rand = randbuffer[vId];
   uint particle_type=0;
   for(uint i=0; i<uNumParticleType-1; i++)
   {
        particle_type+= int( step(particleProportion[i],single_rand) );
   }
   return particle_type;
}
// ----------------------------------------------------------------------------

void PushParticle(in vec3 position,
                  in vec3 velocity,
                  in float age,const uint type)
{
  // Emit particle id.
  const uint id = atomicCounterIncrement(write_count);

#if SPARKLE_USE_SOA_LAYOUT
  positions[id]  = vec4(position, 1.0f);
  velocities[id] = vec4(velocity, 0.0f);
  attributes[id] = vec4(age, age, 0.0f, uintBitsToFloat(id));
#else
  TParticle p;
  p.position = vec4(position, 1.0f);
  p.velocity = vec4(velocity, 0.0f);
  p.start_age = age;
  p.age = age;
  p.id = id;
  p.type = type;
  particles[id] = p;
#endif
}

// ----------------------------------------------------------------------------

void CreateParticle(const uint gid, const uint type) {
  // Random vector.
  const uint rid = 3u * gid;
  const vec3 rn = vec3(randbuffer[rid], randbuffer[rid+1u], randbuffer[rid+2u]);

  // Position
  vec3 pos = EmitterPosition;
  if (EmitterType == 1) {
    //pos += disk_distribution(uEmitterRadius, rn.xy);
    pos += disk_even_distribution(EmitterRadius, gid, uEmitCount);
  } else if (EmitterType == 2) {
    pos += sphere_distribution(EmitterRadius, rn.xy);
  } else if (EmitterType == 3) {
    pos += ball_distribution(EmitterRadius, rn);
  }

  // Velocity
  vec3 vel = EmitterDirection;

  const float single_rand = randbuffer[gid];

  const float age = mix( ParticleMinAge, ParticleMaxAge, single_rand);

  PushParticle(pos, vel, age, type);
}

// ----------------------------------------------------------------------------

layout(local_size_x = PARTICLE_SYSTEM_KERNEL_GROUP_WIDTH) in;
void main() {
  const uint gid = gl_GlobalInvocationID.x;

  if (gid < uEmitCount) {

    uint Type = getParticleType(gid);
    EmitterType = simulationParameters[Type].emitter_type;
    EmitterPosition = simulationParameters[Type].emitter_position.xyz;
    EmitterDirection = simulationParameters[Type].emitter_direction.xyz;
    EmitterRadius = simulationParameters[Type].emitter_radius;
    ParticleMinAge = simulationParameters[Type].min_age;
    ParticleMaxAge = simulationParameters[Type].max_age;

    CreateParticle(gid,Type);    
  }
}

