#ifndef COMMON_GLSL_H
#define COMMON_GLSL_H
#include "../inc/Configuration.h"


/*
* [ IMPORTANT ]
* Data in a ShaderStorage buffer must be layed out using atomic type,
* ie. float[3] instead of vec3, to avoid unwanted padding,
* otherwise use vec4.
* In TParticle position & velocity are vec4 but only their three first
* components are currently used.
*/
struct TParticle 
{
  vec4 position;
  vec4 velocity;
  float start_age;
  float age;
  uint type; 
  uint id;

  /* GPU                                            CPU(8)
  * basealignment       aligned offset              basealignment       aligned offset
    16                  0                           16                  0
    16                  16                          16                  16
    4                   32                          4                   32
    4                   36                          4                   36
    4                   40                          4                   40
    4                   44                          4                   44
  */
};

struct  SSimulationParameters
{
    vec4 emitter_position;
    vec4 emitter_direction;

    unsigned int EmitNumPerSecond;
    float time_step_factor ;
    float min_age;
    float max_age;

    unsigned int emitter_type ;
    float emitter_radius ;
    float scattering_factor;
    float vectorfield_factor;

    float curlnoise_factor;
    float curlnoise_scale;
    float velocity_factor;
    float padding;

    unsigned int enable_scattering;  //由于C++与GPU中的bool对不齐，所以用uint代替
    unsigned int enable_vectorfield;
    unsigned int enable_curlnoise ;
    unsigned int enable_velocity_control;
};
#endif
