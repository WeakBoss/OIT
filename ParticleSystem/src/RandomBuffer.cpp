#include "ParticleSystemPCH.h"
#include "RandomBuffer.h"
#include "ParticleSystemCommon.h"

//**************************************************************************************************
//FUNCTION:
void CRandomBuffer::initialize(unsigned int const vNumElements) {
    m_NumElements = vNumElements;

    glGenBuffers(1u, &m_GLRandomBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_GLRandomBuffer);

    glBufferStorage(GL_SHADER_STORAGE_BUFFER, m_NumElements * sizeof(float), nullptr,GL_MAP_WRITE_BIT);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0u);
}
//**************************************************************************************************
//FUNCTION:
void CRandomBuffer::deinitialize() {
    glDeleteBuffers(1u, &m_GLRandomBuffer);
}
//**************************************************************************************************
//FUNCTION:
void CRandomBuffer::generateValues() {
    std::uniform_real_distribution<float> distrib(m_Min, m_Max);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_GLRandomBuffer);

    float* buffer = reinterpret_cast<float*>(glMapBufferRange(
        GL_SHADER_STORAGE_BUFFER, 0u, m_NumElements * sizeof(float),
        GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT
    ));

    for (unsigned int i = 0u; i < m_NumElements; ++i) {
        buffer[i] = distrib(m_Mt19937);
    }

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0u);

}
//**************************************************************************************************
//FUNCTION:
void CRandomBuffer::bind() {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_RANDOM_VALUES, m_GLRandomBuffer);
}
//**************************************************************************************************
//FUNCTION:
void CRandomBuffer::unbind() {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_RANDOM_VALUES, 0u);
}
