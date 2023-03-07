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
    const float DeltaTime = App::getDeltaTime();
    m_pGPUPartile->update(DeltaTime, glm::mat4x4());
}