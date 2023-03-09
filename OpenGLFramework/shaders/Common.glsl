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
  float _padding0;//删掉_padding会导致出问题，可能是因为GPU内部的对齐和CPU不一致造成的
  uint id;
};
#endif
