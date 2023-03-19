#pragma once
#include "ParticleOITPCH.h"
#include "ParticleSystemObj.h"
#include "Grid.h"
class CDirectRenderPass : public IRenderPass
{

public:
    CDirectRenderPass(const std::string& vPassName,int vExecutionOrder):IRenderPass(vPassName,vExecutionOrder){}
    ~CDirectRenderPass() 
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RenderParametersBuffer);
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0u);
        glDeleteBuffers(1u, &m_RenderParametersBuffer);
    }

    void initV() override;
    void updateV() override;
private:
    void genSmokeTexture();

    std::shared_ptr<CParticleSystemObj> m_pParticleSystemObj = nullptr;
    std::shared_ptr<CGridObj> m_pGridObj = nullptr;

    std::shared_ptr<hiveGraphics::STexture> m_ParticleTexture = nullptr;
 
    
    GLuint m_RenderParametersBuffer;
    SRenderParameters* m_pMapRenderParameters;

    std::shared_ptr<CShader> m_pRenderPointSpriteShader = nullptr;
    std::shared_ptr<CShader> m_pRenderGridShader = nullptr;
    GLuint m_IndirectBuffer;

    void genRenderParametersBuffer();
    void bindRenderParameters();

};
