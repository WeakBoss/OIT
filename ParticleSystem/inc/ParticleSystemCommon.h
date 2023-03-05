#ifndef PARTICLE_SYSTEM_Common_H
#define PARTICLE_SYSTEM_Common_H

#define MAX_NUM_PARTICLES   (1u << 18u)
#define PARTICLE_SYSTEM_KERNEL_GROUP_WIDTH       (512u)
#define MAX_NUM_PARTICLES_PER_BATCH std::max(256u, (MAX_NUM_PARTICLES >> 4u));

#define STORAGE_BINDING_PARTICLES_FIRST                  0
#define STORAGE_BINDING_PARTICLES_SECOND                 1
#define STORAGE_BINDING_RANDOM_VALUES                    2
#define STORAGE_BINDING_INDIRECT_ARGS                    3

//#define STORAGE_BINDING_DOT_PRODUCTS                     4
//#define STORAGE_BINDING_INDICES_FIRST                    5
//#define STORAGE_BINDING_INDICES_SECOND                   6
//#define COUNT_STORAGE_BINDING                            7

#define ATOMIC_COUNTER_BINDING_FIRST                     0
#define ATOMIC_COUNTER_BINDING_SECOND                    1

typedef unsigned int  GLuint;
typedef int  GLint;
#ifdef __cplusplus
#include <glm/glm.hpp>
using namespace glm;


enum EmitterType
{
    EMITTER_POINT,
    EMITTER_DISK,
    EMITTER_SPHERE,
    EMITTER_BALL,
    kNumEmitterType
};

enum SimulationVolume 
{
    VOLUME_SPHERE,
    VOLUME_BOX,
    VOLUME_NONE,
    kNumSimulationVolume
};

struct SimulationParameters_t
{
    float time_step_factor = 1.0f;
    float min_age = 50.0f;
    float max_age = 100.0f;
    EmitterType emitter_type = EmitterType::EMITTER_SPHERE;
    float emitter_position[3] = { 0.0f, 0.0f, 0.0f };
    float emitter_direction[3] = { 0.0f, 1.0f, 0.0f };
    float emitter_radius = 32.0f;
    SimulationVolume bounding_volume = SimulationVolume::VOLUME_SPHERE;
    float bounding_volume_size = 256.0f;

    float scattering_factor = 1.0f;
    float vectorfield_factor = 1.0f;
    float curlnoise_factor = 16.0f;
    float curlnoise_scale = 128.0f;
    float velocity_factor = 8.0f;

    bool enable_scattering = false;
    bool enable_vectorfield = false;
    bool enable_curlnoise = true;
    bool enable_velocity_control = true;
};

enum RenderMode 
{
    RENDERMODE_STRETCHED,
    RENDERMODE_POINTSPRITE,
    kNumRenderMode
};

enum ColorMode
{
    COLORMODE_DEFAULT,
    COLORMODE_GRADIENT,
    kNumColorMode
};

struct RenderingParameters_t 
{
    RenderMode rendermode = RENDERMODE_STRETCHED;
    float stretched_factor = 10.0f;
    ColorMode colormode = COLORMODE_DEFAULT;
    float birth_gradient[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
    float death_gradient[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
    float min_size = 0.75f;
    float max_size = 25.0f;
    float fading_factor = 0.35f;
}; 

void SwapUint(unsigned int& a, unsigned int& b)
{
    a ^= b;
    b ^= a;
    a ^= b;
}

unsigned int GetNumTrailingBits(unsigned int const n)
{
    unsigned int r = 0u;
    for (unsigned int i = n; i > 1u; i >>= 1u) ++r;
    return r;
}

#endif

struct SParticle 
{
    vec4 Position;
    vec4 Velocity;
    float BirthTime;
    float LifeSpan;
    uint Id;
};

#endif