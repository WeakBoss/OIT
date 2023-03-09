#pragma once
#include "ParticleOITPCH.h"
#include "ParticleSystem.h"


class CParticleSystemObj: public IGameObject
{
public:
    CParticleSystemObj(const std::string& vGameObjectName, int vExecutionOrder):IGameObject(vGameObjectName,vExecutionOrder)
    {
        m_pGPUPartile = std::make_shared<hiveGraphics::CParticleSystem>();
    }
    ~CParticleSystemObj() = default;
    void initV() override;
    void updateV() override;
    void use()const { glBindVertexArray(m_VAO); }
    unsigned int getIndirectBuffer() const { return m_pGPUPartile->getIndirectBuffer();}
    void setSimulationParameters(const SSimulationParameters& vSimulationParameters)
    {
        m_pGPUPartile->setSimulationParameters(vSimulationParameters);
    }
    const SSimulationParameters& getSimulationParameters() const
    {
        return m_pGPUPartile->getSimulationParameters();
    }
private:
    GLuint m_VAO;
    std::shared_ptr<hiveGraphics::CParticleSystem> m_pGPUPartile;
};