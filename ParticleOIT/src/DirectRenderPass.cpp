#include "ParticleOITPCH.h"
#include "DirectRenderPass.h"

//**************************************************************************************************
//FUNCTION:
void CDirectRenderPass::initV()
{
    m_pParticleSystemObj = std::dynamic_pointer_cast<CParticleSystemObj>(ResourceManager::getGameObjectByName("ParticleSystem"));
    m_pParticleSystemObj->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    m_pRenderPointSpriteShader = std::make_shared<CShader>("shaders/particle_vs.glsl", "shaders/particle_fs.glsl");
    m_IndirectBuffer = m_pParticleSystemObj->getIndirectBuffer();

    m_pGridObj = std::dynamic_pointer_cast<CGridObj>(ResourceManager::getGameObjectByName("Grid"));
    m_pGridObj->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    m_pRenderGridShader = std::make_shared<CShader>("shaders/vs_grid.glsl", "shaders/fs_grid.glsl");
   
    genRenderParametersBuffer();
    genSmokeTexture();

    m_FloorTexture = std::make_shared<hiveGraphics::STexture>();
    m_FloorTexture->TextureType = STexture::ETextureType::Texture2D;
    loadTextureFromFile("textures/floor3.jpg", m_FloorTexture);

}
//**************************************************************************************************
//FUNCTION:
void CDirectRenderPass::updateV()
{
    glClearColor(0.155f, 0.15f, 0.13f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    m_pRenderGridShader->activeShader();
    m_pGridObj->use();
    m_pRenderGridShader->setTextureUniformValue("uFloorTexture", m_FloorTexture);
    m_pRenderGridShader->setFloatUniformValue("u_ScaleFator",  m_pGridObj->getScaleFactor());
    m_pRenderGridShader->setMat4UniformValue("u_Model", glm::value_ptr(m_pGridObj->getModelMatrix()));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0u);
    glUseProgram(0u);


    
    m_pRenderPointSpriteShader->activeShader();
    m_pRenderPointSpriteShader->setTextureUniformValue("uSmokeTexture", m_ParticleTexture);

    bindRenderParameters();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_ALWAYS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);

    glEnable(GL_PROGRAM_POINT_SIZE);
    m_pParticleSystemObj->use();
 
    void const* offset = reinterpret_cast<void const*>(offsetof(TIndirectValues, DrawCount));
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_IndirectBuffer);
    glDrawArraysIndirect(GL_POINTS, offset); 
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0u);
    glBindVertexArray(0u);

    glUseProgram(0u);
}
 
//**************************************************************************************************
//FUNCTION:
void CDirectRenderPass::genSmokeTexture()
{
    m_ParticleTexture = std::make_shared<hiveGraphics::STexture>();
    m_ParticleTexture->InternalFormat = GL_RGBA;
    m_ParticleTexture->ExternalFormat = GL_RGBA;
    m_ParticleTexture->TextureType = STexture::ETextureType::Texture2DArray;
    m_ParticleTexture->TextureAttachmentType = STexture::ETextureAttachmentType::ColorTexture;
    const std::vector<std::string> TexturePath = { "textures/smoke2.png","textures/smoke1.png" };
    m_ParticleTexture->Depth = TexturePath.size();
    loadTextureFromFile(TexturePath,m_ParticleTexture);
   
}

//**************************************************************************************************
//FUNCTION:
void CDirectRenderPass::genRenderParametersBuffer()
{
    glGenBuffers(1u, &m_RenderParametersBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RenderParametersBuffer);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(SRenderParameters) * m_pParticleSystemObj->getNumParticleType(), nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    m_pMapRenderParameters = reinterpret_cast<SRenderParameters*>(
        glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(SRenderParameters) * m_pParticleSystemObj->getNumParticleType(), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT));
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0u);
  
    
 }
//**************************************************************************************************
//FUNCTION:
void CDirectRenderPass::bindRenderParameters()
{
    auto  RenderParametesSet = ResourceManager::getSharedDataByName<std::vector<SRenderParameters>>("DirectRenderSimulationParameters");
    _WRONG(m_pMapRenderParameters == nullptr,"Empty Ptr of Buffer£¡");
    std::memcpy(m_pMapRenderParameters, RenderParametesSet.data(), sizeof(SRenderParameters) * m_pParticleSystemObj->getNumParticleType());
    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, STORAGE_RENDER_PARAMETERS, m_RenderParametersBuffer);
}
