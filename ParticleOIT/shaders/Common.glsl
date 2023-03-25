#ifndef COMMON_GLSL_H
#define COMMON_GLSL_H
#include "../inc/Configuration.h"

struct SRenderParameters
{
    float min_size;
    float max_size;
    float size_scale_factor;
    float alpha_factor;
    vec4 particle_color;

};
#endif