#include "AppendConsumeBuffer.h"
#include <GL/glew.h>
#include <random>
#include "ParticleSystemCommon.h"

//**************************************************************************************************
//FUNCTION:
void AppendConsumeBuffer::init() {
    /* Storage buffers */
    glGenBuffers(2u, m_StorageBuffers);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_StorageBuffers[0u]);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, m_StorageBufferSize, nullptr, 0);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_StorageBuffers[1u]);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, m_StorageBufferSize, nullptr, 0);

    /* Atomic Counter buffers */
    GLuint const default_values[2u] = { 0u, 0u };
    glGenBuffers(2u, m_AtomicBuffers);

    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, m_AtomicBuffers[0u]);
    glBufferStorage(GL_ATOMIC_COUNTER_BUFFER, sizeof default_values[0u], &default_values[0u],
        GL_MAP_READ_BIT
    );
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, m_AtomicBuffers[1u]);
    glBufferStorage(GL_ATOMIC_COUNTER_BUFFER, sizeof default_values[1u], &default_values[1u],
        GL_MAP_READ_BIT
    );

    /* Unbind buffers */
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0u);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0u);
 
}

//**************************************************************************************************
//FUNCTION:
void AppendConsumeBuffer::deinit() {
    glDeleteBuffers(2u, m_StorageBuffers);
    glDeleteBuffers(2u, m_AtomicBuffers);
}
//**************************************************************************************************
//FUNCTION:
void AppendConsumeBuffer::bind() {
    bindAttributes();
    bindAtomics();
}
//**************************************************************************************************
//FUNCTION:
void AppendConsumeBuffer::unbind() {
    unbindAttributes();
    unbindAtomics();
}
//**************************************************************************************************
//FUNCTION:
void AppendConsumeBuffer::bindAttributes() {
#if SPARKLE_USE_SOA_LAYOUT
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_PARTICLE_POSITIONS_A, m_StorageBuffers[0u], 0 * single_attrib_buffer_size_, single_attrib_buffer_size_);
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_PARTICLE_VELOCITIES_A, m_StorageBuffers[0u], 1 * single_attrib_buffer_size_, single_attrib_buffer_size_);
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_PARTICLE_ATTRIBUTES_A, m_StorageBuffers[0u], 2 * single_attrib_buffer_size_, single_attrib_buffer_size_);

    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_PARTICLE_POSITIONS_B, m_StorageBuffers[1u], 0 * single_attrib_buffer_size_, single_attrib_buffer_size_);
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_PARTICLE_VELOCITIES_B, m_StorageBuffers[1u], 1 * single_attrib_buffer_size_, single_attrib_buffer_size_);
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_PARTICLE_ATTRIBUTES_B, m_StorageBuffers[1u], 2 * single_attrib_buffer_size_, single_attrib_buffer_size_);
#else
    glBindBuffersBase(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_PARTICLES_FIRST, 2u, m_StorageBuffers);
#endif
}
//**************************************************************************************************
//FUNCTION:
void AppendConsumeBuffer::unbindAttributes() {
#if SPARKLE_USE_SOA_LAYOUT
    glBindBuffersBase(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_PARTICLE_POSITIONS_A, attrib_buffer_count_, nullptr);
    glBindBuffersBase(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_PARTICLE_POSITIONS_B, attrib_buffer_count_, nullptr);
#else
    glBindBuffersBase(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_PARTICLES_FIRST, 2u, nullptr); //
#endif
}
//**************************************************************************************************
//FUNCTION:
void AppendConsumeBuffer::bindAtomics() {
    //注意bindBuffers  ssss
   //这里应该是ATOMIC_COUNTER_BINDING_FIRST作为绑定点的开始，所以第二个绑定点也绑定了
    glBindBuffersBase(GL_ATOMIC_COUNTER_BUFFER, ATOMIC_COUNTER_BINDING_FIRST, 2u, m_AtomicBuffers);
}
//**************************************************************************************************
//FUNCTION:
void AppendConsumeBuffer::unbindAtomics() {
    glBindBuffersBase(GL_ATOMIC_COUNTER_BUFFER, ATOMIC_COUNTER_BINDING_FIRST, 2u, nullptr);
}
//**************************************************************************************************
//FUNCTION:
void AppendConsumeBuffer::swapAtomics() {
    hiveGraphics::SwapUint(m_AtomicBuffers[0u], m_AtomicBuffers[1u]);
}
//**************************************************************************************************
//FUNCTION:
void AppendConsumeBuffer::swapStorage() {
     
#if 1
    // Costly but safe.   由于VBO用的是fist，所以不能用swapuint
    glCopyNamedBufferSubData(
        m_StorageBuffers[1u], m_StorageBuffers[0u],
        0u, 0u, m_StorageBufferSize);
#else
    // Cheapest alternative but can makes some particle flicks
    // due to non updated rendering params (workaround possible).
    SwapUint(m_StorageBuffers[0u], m_StorageBuffers[1u]);
#endif
}
 
//**************************************************************************************************
//FUNCTION:
unsigned int AppendConsumeBuffer::getNumAliveParticlesFromDevice() const {
    GLuint num_alived_particles(0u);

    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, m_AtomicBuffers[1u]);
    /// @warning most costly call.
    GLuint* ptr = reinterpret_cast<GLuint*>(glMapBuffer(GL_ATOMIC_COUNTER_BUFFER, GL_READ_ONLY));
    num_alived_particles = *ptr;
    glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0u);

    return num_alived_particles;
}
