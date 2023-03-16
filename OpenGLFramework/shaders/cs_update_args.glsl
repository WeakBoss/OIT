#version 450 core

// ============================================================================

/*
 * Update a dispatch buffer argument value to match a given group size.
 */

// ============================================================================


#include "Common.glsl"

layout(binding = ATOMIC_COUNTER_BINDING_FIRST)
uniform atomic_uint read_count;

layout(std430, binding = STORAGE_BINDING_INDIRECT_ARGS)
writeonly buffer IndirectArgs {
  uint dispatch_x;
  uint dispatch_y;
  uint dispatch_z;   //应该simulation只用到了前三个
  uint draw_count;   
  uint draw_primCount;
  uint draw_first;
  uint draw_reserved;
  unsigned int Padding;
};

layout(local_size_x = 1u) in;
void main() {
  const uint num_particles = atomicCounter(read_count);
  dispatch_x = (num_particles + PARTICLE_SYSTEM_KERNEL_GROUP_WIDTH - 1u) / PARTICLE_SYSTEM_KERNEL_GROUP_WIDTH;

  /// @note
  /// not the real value (one frame of accuracy lost), but way cheaper than using
  /// device-host sync with glCopyNamedBufferSubData in postprocess stage.
  //draw_count = num_particles;
}
