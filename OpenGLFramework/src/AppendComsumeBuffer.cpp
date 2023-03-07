#include "AppendConsumeBuffer.h"
#include <GL/glew.h>
#include <random>
#include "ParticleSystemCommon.h"
void AppendConsumeBuffer::initialize() {
    /* Storage buffers */
    glGenBuffers(2u, gl_storage_buffer_ids_);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, gl_storage_buffer_ids_[0u]);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, storage_buffer_size_, nullptr, 0);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, gl_storage_buffer_ids_[1u]);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, storage_buffer_size_, nullptr, 0);

    /* Atomic Counter buffers */
    GLuint const default_values[2u] = { 0u, 0u };
    glGenBuffers(2u, gl_atomic_buffer_ids_);

    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, gl_atomic_buffer_ids_[0u]);
    glBufferStorage(GL_ATOMIC_COUNTER_BUFFER, sizeof default_values[0u], &default_values[0u],
        GL_MAP_READ_BIT
    );
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, gl_atomic_buffer_ids_[1u]);
    glBufferStorage(GL_ATOMIC_COUNTER_BUFFER, sizeof default_values[1u], &default_values[1u],
        GL_MAP_READ_BIT
    );

    /* Unbind buffers */
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0u);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0u);
 
}

void AppendConsumeBuffer::deinitialize() {
    glDeleteBuffers(2u, gl_storage_buffer_ids_);
    glDeleteBuffers(2u, gl_atomic_buffer_ids_);
 
}

void AppendConsumeBuffer::bind() {
    bind_attributes();
    bind_atomics();
}

void AppendConsumeBuffer::unbind() {
    unbind_attributes();
    unbind_atomics();
}

void AppendConsumeBuffer::bind_attributes() {
#if SPARKLE_USE_SOA_LAYOUT
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_PARTICLE_POSITIONS_A, gl_storage_buffer_ids_[0u], 0 * single_attrib_buffer_size_, single_attrib_buffer_size_);
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_PARTICLE_VELOCITIES_A, gl_storage_buffer_ids_[0u], 1 * single_attrib_buffer_size_, single_attrib_buffer_size_);
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_PARTICLE_ATTRIBUTES_A, gl_storage_buffer_ids_[0u], 2 * single_attrib_buffer_size_, single_attrib_buffer_size_);

    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_PARTICLE_POSITIONS_B, gl_storage_buffer_ids_[1u], 0 * single_attrib_buffer_size_, single_attrib_buffer_size_);
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_PARTICLE_VELOCITIES_B, gl_storage_buffer_ids_[1u], 1 * single_attrib_buffer_size_, single_attrib_buffer_size_);
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_PARTICLE_ATTRIBUTES_B, gl_storage_buffer_ids_[1u], 2 * single_attrib_buffer_size_, single_attrib_buffer_size_);
#else
    glBindBuffersBase(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_PARTICLES_FIRST, 2u, gl_storage_buffer_ids_);
#endif
}

void AppendConsumeBuffer::unbind_attributes() {
#if SPARKLE_USE_SOA_LAYOUT
    glBindBuffersBase(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_PARTICLE_POSITIONS_A, attrib_buffer_count_, nullptr);
    glBindBuffersBase(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_PARTICLE_POSITIONS_B, attrib_buffer_count_, nullptr);
#else
    glBindBuffersBase(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_PARTICLES_FIRST, 2u, nullptr); //
#endif
}

void AppendConsumeBuffer::bind_atomics() {
    //注意bindBuffers  ssss
   //这里应该是ATOMIC_COUNTER_BINDING_FIRST作为绑定点的开始，所以第二个绑定点也绑定了
    glBindBuffersBase(GL_ATOMIC_COUNTER_BUFFER, ATOMIC_COUNTER_BINDING_FIRST, 2u, gl_atomic_buffer_ids_);
}

void AppendConsumeBuffer::unbind_atomics() {
    glBindBuffersBase(GL_ATOMIC_COUNTER_BUFFER, ATOMIC_COUNTER_BINDING_FIRST, 2u, nullptr);
}

void AppendConsumeBuffer::swap_atomics() {
    hiveGraphics::SwapUint(gl_atomic_buffer_ids_[0u], gl_atomic_buffer_ids_[1u]);
}

void AppendConsumeBuffer::swap_storage() {
#if 1
    // Costly but safe.
    glCopyNamedBufferSubData(
        gl_storage_buffer_ids_[1u], gl_storage_buffer_ids_[0u],
        0u, 0u, storage_buffer_size_);
#else
    // Cheapest alternative but can makes some particle flicks
    // due to non updated rendering params (workaround possible).
    SwapUint(gl_storage_buffer_ids_[0u], gl_storage_buffer_ids_[1u]);
#endif
}

unsigned int AppendConsumeBuffer::get_num_alive_particles_from_device() {
    GLuint num_alived_particles(0u);

    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, gl_atomic_buffer_ids_[1u]);
    /// @warning most costly call.
    GLuint* ptr = reinterpret_cast<GLuint*>(glMapBuffer(GL_ATOMIC_COUNTER_BUFFER, GL_READ_ONLY));
    num_alived_particles = *ptr;
    glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0u);
 
    return num_alived_particles;
}
