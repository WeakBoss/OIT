#pragma once
#include "ParticleOITPCH.h"
#include "ParticleSystemObj.h"
#include "Grid.h"
class CDirectRenderPass : public IRenderPass
{

public:
    CDirectRenderPass(const std::string& vPassName,int vExecutionOrder):IRenderPass(vPassName,vExecutionOrder){}
    ~CDirectRenderPass() {}

    void initV() override;
    void updateV() override;
private:
    std::shared_ptr<CParticleSystemObj> m_pParticleSystemObj = nullptr;
    std::shared_ptr<CGridObj> m_pGridObj = nullptr;

    RenderingParameters_t m_RenderingParameters;
    std::shared_ptr<CShader> m_pRenderPointSpriteShader = nullptr;
    std::shared_ptr<CShader> m_pRenderGridShader = nullptr;
    GLuint m_IndirectBuffer;
};
