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
    void setSimulationParameters(const SSimulationParameters& vSimulationParameters, int vIndex)
    {
        m_pGPUPartile->setSimulationParameters(vSimulationParameters,vIndex);
    }
    void setSimulationParameters(const std::vector<SSimulationParameters>& vSimulationParameters)
    {
        m_pGPUPartile->setSimulationParameters(vSimulationParameters);
    }
    void addParticleType(const SSimulationParameters& vSimulationParams)
    {
        m_pGPUPartile->addParticleType(vSimulationParams);
    }
    void addParticleType(const std::vector<SSimulationParameters>& vSimulationParamsSet)
    {
        m_pGPUPartile->addParticleType(vSimulationParamsSet);
    }
    unsigned int getNumAliveParticles()
    {
        return m_pGPUPartile->getNumAliveParticles();
    }
    unsigned int getNumParticleType()
    {
        return m_pGPUPartile->getNumParticleType();
    }
private:
    GLuint m_VAO;
    std::shared_ptr<hiveGraphics::CParticleSystem> m_pGPUPartile;
    std::vector<SSimulationParameters> m_SimulationParametersSet;
};