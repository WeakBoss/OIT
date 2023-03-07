#pragma once
#include "ParticleOITPCH.h"
#include "ParticleSystem.h"


class CParticleSystemObj: public IGameObject
{
public:
    CParticleSystemObj(const std::string& vGameObjectName, int vExecutionOrder):IGameObject(vGameObjectName,vExecutionOrder)
    {
        m_pGPUPartile = std::make_shared<hiveGraphics::GPUParticle>();
    }
    ~CParticleSystemObj() = default;
    void initV() override;
    void updateV() override;
    void use()const { glBindVertexArray(m_VAO); }
    unsigned int getIndirectBuffer() const { return m_pGPUPartile->getIndirectBuffer();}
private:
    GLuint m_VAO;
    std::shared_ptr<hiveGraphics::GPUParticle> m_pGPUPartile;
};