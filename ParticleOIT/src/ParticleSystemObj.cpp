#include "ParticleOITPCH.h"
#include "ParticleSystemObj.h"

//**************************************************************************************************
//FUNCTION:
void CParticleSystemObj::initV()
{
    SSimulationParameters SP1, SP2;
    SP2.emitter_position = glm::vec4(30.0f, 0.0f, 0.0f,0.0f);
    SP2.emitter_position = glm::vec4(50.0f, 0.0f, 0.0f, 0.0f);
    SP2.EmitNumPerSecond *= 5;
    m_pGPUPartile->addParticleType(SP2);
    m_pGPUPartile->addParticleType(SP1);

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