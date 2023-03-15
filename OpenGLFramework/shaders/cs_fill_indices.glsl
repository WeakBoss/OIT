#version 450 core

/* Fill a buffer with continuous indices, used in the sorting stage. */

#include "Common.glsl"

layout(std430, binding = STORAGE_BINDING_INDICES_FIRST)
writeonly buffer IndexBuffer {
  uint indices[];
};

layout(local_size_x = PARTICLE_SYSTEM_KERNEL_GROUP_WIDTH) in;
void main() {
  uint tid = gl_GlobalInvocationID.x;
  indices[tid] = tid;
}
