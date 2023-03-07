#include "ParticleOITPCH.h"
#include "ParticleSystemObj.h"


//**************************************************************************************************
//FUNCTION:
void CParticleSystemObj::initV()
{
    m_pGPUPartile->init();
    m_VAO = m_pGPUPartile->getParticlePositionVAO();
}

//**************************************************************************************************
//FUNCTION:
void CParticleSystemObj::updateV()
{
    m_pGPUPartile->update(0.05f, glm::mat4x4());
}