#include "RandomBuffer.h"
#include <GL/glew.h>
#include "ParticleSystemCommon.h"

//**************************************************************************************************
//FUNCTION:
void CRandomBuffer::init(unsigned int const vNumElements) {
    m_NumElements = vNumElements + m_extraCapacity;

    glGenBuffers(1u, &m_GLRandomBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_GLRandomBuffer);

    glBufferStorage(GL_SHADER_STORAGE_BUFFER, m_NumElements * sizeof(float), nullptr,GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0u);
}
//**************************************************************************************************
//FUNCTION:
void CRandomBuffer::deinit() {
    glDeleteBuffers(1u, &m_GLRandomBuffer);
}
//**************************************************************************************************
//FUNCTION:
void CRandomBuffer::generateValues() {
    std::uniform_real_distribution<float> distrib(m_Min, m_Max);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_GLRandomBuffer);

    float* buffer = reinterpret_cast<float*>(glMapBufferRange(
        GL_SHADER_STORAGE_BUFFER, 0u, m_NumElements * sizeof(float),
        GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT
    ));

    for (unsigned int i = 0u; i < m_NumElements; ++i) {
        buffer[i] = distrib(m_Mt19937);
    }

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0u);

}

//**************************************************************************************************
//FUNCTION:
int CRandomBuffer::getRandomInt(int vMin, int vMax)
{
    std::uniform_int_distribution<int> distrib(vMin, vMax);
    return distrib(m_Mt19937);
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
