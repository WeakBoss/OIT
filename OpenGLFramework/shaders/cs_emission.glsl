#version 450 core

 #include "Common.glsl"
#include "inc_math.glsl"

uint EmitterType;
vec3 EmitterPosition;
vec3 EmitterDirection;
float EmitterRadius;
float ParticleMinAge;
float ParticleMaxAge;

layout(location=0) uniform uint uEmitCount;
layout(location=1) uniform uint uNumParticleType;
layout(location=2) uniform uint uRandomuint;


layout(binding = ATOMIC_COUNTER_BINDING_FIRST)
uniform atomic_uint write_count;

 
layout(std430, binding = STORAGE_BINDING_PARTICLES_FIRST)
writeonly buffer ParticleBufferA {
  TParticle particles[];
};

 

layout(std430, binding = STORAGE_BINDING_RANDOM_VALUES)
readonly buffer RandomBuffer {
  float randbuffer[];
};

layout(std430, binding = STORAGE_SIMULATE_PARAMETER)
readonly buffer SimulationParameters{
   SSimulationParameters simulationParameters[];
};

layout(std430, binding = STORAGE_PARTICLE_PROPORTION)
readonly buffer ParticleProportion{
   float particleProportion[];
};

 
uint getParticleType(uint vId)
{
   const float single_rand = randbuffer[vId + uRandomuint];
   uint particle_type=0;
   for(uint i=0; i<uNumParticleType-1; i++)
   {
        particle_type+= int( step(particleProportion[i],single_rand) );
   }
   return particle_type;
}
 
void PushParticle(in vec3 position,in vec3 velocity, in float age,const uint type)
{
  const uint id = atomicCounterIncrement(write_count);
  TParticle p;
  p.position = vec4(position, 1.0f);
  p.velocity = vec4(velocity, 0.0f);
  p.start_age = age;
  p.age = age;
  p.type = type;
  p.id = id;
  particles[id] = p;
}

void CreateParticle(const uint gid, const uint type) {
  // Random vector.
  const uint rid = 3u * gid + uRandomuint;
  const vec3 rn = vec3(randbuffer[rid], randbuffer[rid+1u], randbuffer[rid+2u]);

  // Position
  vec3 pos = EmitterPosition;
  vec3 randomPos = vec3(0.0f);

  if (EmitterType == 1) {
    randomPos += disk_even_distribution(EmitterRadius, gid , uEmitCount, randbuffer[rid + uRandomuint]);
  } else if (EmitterType == 2) {
    randomPos += sphere_distribution(EmitterRadius, rn.xy);
  } else if (EmitterType == 3) {
    randomPos += ball_distribution(EmitterRadius, rn);
  }
  pos += randomPos;
  // Velocity
  vec3 vel = normalize( randomPos + EmitterDirection * 5 );

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

