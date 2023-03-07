#pragma once
#include "ParticleOITPCH.h"
#include "ParticleSystemObj.h"
class CDirectRenderPass : public IRenderPass
{

public:
    CDirectRenderPass(const std::string& vPassName,int vExecutionOrder):IRenderPass(vPassName,vExecutionOrder){}
    ~CDirectRenderPass() {}

    void initV() override;
    void updateV() override;
private:
    std::shared_ptr<CParticleSystemObj> m_pParticleSystemObj = nullptr;
    RenderingParameters_t m_RenderingParameters;
    std::shared_ptr<CShader> m_pRenderPointSpriteShader = nullptr;
    GLuint m_IndirectBuffer;

};
