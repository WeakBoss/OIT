#include "ParticleOITPCH.h"
#include "DirectRenderPass.h"

//**************************************************************************************************
//FUNCTION:
void CDirectRenderPass::initV()
{
    m_pParticleSystemObj = std::dynamic_pointer_cast<CParticleSystemObj>(ResourceManager::getGameObjectByName("ParticleSystem"));
    m_pParticleSystemObj->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    m_pRenderPointSpriteShader = std::make_shared<CShader>("shaders/vs_generic.glsl", "shaders/fs_point_sprite.glsl");
    m_IndirectBuffer = m_pParticleSystemObj->getIndirectBuffer();

    m_pGridObj = std::dynamic_pointer_cast<CGridObj>(ResourceManager::getGameObjectByName("Grid"));
    m_pGridObj->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    m_pRenderGridShader = std::make_shared<CShader>("shaders/vs_grid.glsl", "shaders/fs_grid.glsl");
   

}
//**************************************************************************************************
//FUNCTION:
void CDirectRenderPass::updateV()
{
    glClearColor(0.155f, 0.15f, 0.13f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    m_pRenderGridShader->activeShader();
   
    m_pGridObj->use();
    m_pRenderGridShader->setFloatUniformValue("uScaleFactor", m_pGridObj->getScaleFactor());
    glDrawArrays(GL_LINES, 0, m_pGridObj->getNumVertices());
    glBindVertexArray(0u);
    glUseProgram(0u);


    m_RenderingParameters = ResourceManager::getSharedDataByName<RenderingParameters_t>("DirectRenderSimulationParameters");
    switch (m_RenderingParameters.rendermode) {
    case RENDERMODE_STRETCHED:
        break;

    case RENDERMODE_POINTSPRITE:
        m_pRenderPointSpriteShader->activeShader();
        m_pRenderPointSpriteShader->setFloatUniformValue("uMinParticleSize", m_RenderingParameters.min_size);
        m_pRenderPointSpriteShader->setFloatUniformValue("uMaxParticleSize", m_RenderingParameters.max_size);
        m_pRenderPointSpriteShader->setFloatUniformValue("uColorMode", m_RenderingParameters.colormode);
        m_pRenderPointSpriteShader->setFloatUniformValue("uBirthGradient", 
            m_RenderingParameters.birth_gradient[0],
            m_RenderingParameters.birth_gradient[1],
            m_RenderingParameters.birth_gradient[2]);
        m_pRenderPointSpriteShader->setFloatUniformValue("uDeathGradient",
            m_RenderingParameters.death_gradient[0],
            m_RenderingParameters.death_gradient[1],
            m_RenderingParameters.death_gradient[2]);
        m_pRenderPointSpriteShader->setFloatUniformValue("uFadeCoefficient", m_RenderingParameters.fading_factor);

        break;
    default:
        break;
    }

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    m_pParticleSystemObj->use();
    void const* offset = reinterpret_cast<void const*>(offsetof(TIndirectValues, draw_count));
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_IndirectBuffer);
    glDrawArraysIndirect(GL_POINTS, offset); 
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0u);
    glBindVertexArray(0u);

    glUseProgram(0u);
}