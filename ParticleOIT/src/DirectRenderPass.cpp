#include "ParticleOITPCH.h"
#include "DirectRenderPass.h"

//**************************************************************************************************
//FUNCTION:
void CDirectRenderPass::initV()
{
    m_pParticleSystemObj = std::dynamic_pointer_cast<CParticleSystemObj>(ResourceManager::getGameObjectByName("ParticleSystem"));
    m_pRenderPointSpriteShader = std::make_shared<CShader>("shaders/vs_generic.glsl", "shaders/fs_point_sprite.glsl");
    m_IndirectBuffer = m_pParticleSystemObj->getIndirectBuffer();
}
//**************************************************************************************************
//FUNCTION:
void CDirectRenderPass::updateV()
{
    switch (m_RenderingParameters.rendermode) {
    case RENDERMODE_STRETCHED:
        m_pRenderPointSpriteShader->activeShader();
       /* glUniform1f(ulocation_.render_stretched_sprite.colorMode, rendering_params_.colormode);
        glUniform3fv(ulocation_.render_stretched_sprite.birthGradient, 1, rendering_params_.birth_gradient);
        glUniform3fv(ulocation_.render_stretched_sprite.deathGradient, 1, rendering_params_.death_gradient);
        glUniform1f(ulocation_.render_stretched_sprite.spriteStretchFactor, rendering_params_.stretched_factor);
        glUniform1f(ulocation_.render_stretched_sprite.fadeCoefficient, rendering_params_.fading_factor);*/
        break;

    case RENDERMODE_POINTSPRITE:
        break;
    default:
        break;
    }
    glClearColor(0.155f, 0.15f, 0.13f, 1.0f);
    glDisable(GL_DEPTH_TEST);
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