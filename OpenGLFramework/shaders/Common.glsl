#ifndef COMMON_GLSL_H
#define COMMON_GLSL_H
#include "../inc/Configuration.h"

struct TParticle 
{
    vec4 position;
    vec4 velocity;
    float start_age;
    float age;
    uint id;
};
#endif
