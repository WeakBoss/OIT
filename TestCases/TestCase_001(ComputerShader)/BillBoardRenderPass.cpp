#include "pch.h"
#include "BillboardRenderPass.h"
#include "MethodDef.h"
//**************************************************************************************************
//FUNCTION:
void CBillBoardRenderPass::initV()
{
    m_pQuadObj = std::dynamic_pointer_cast<CQuadObj>(ResourceManager::getGameObjectByName("Quad"));
    m_pQuadObj->setPosition(glm::vec3(-2.0f, -2.0f, 0.0f));
    m_pRenderQuadShader = std::make_shared<CShader>("vs_grid.glsl", "fs_grid.glsl");
    
}
//**************************************************************************************************
//FUNCTION:
void CBillBoardRenderPass::updateV()
{
    glClearColor(0.155f, 0.15f, 0.13f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_pRenderQuadShader->activeShader();
    m_pQuadObj->use();
   
    m_pRenderQuadShader->setMat4UniformValue("model", glm::value_ptr(m_pQuadObj->getModelMatrix()));
    
    glm::mat4 BillBoardRotate = glm::mat4(1);
#ifdef  Screen_Aligned_Billboard
    glm::vec3  Up = glm::normalize(Camera::getMainCameraUp());
    glm::vec3 Front = glm::normalize(-Camera::getMainCameraFront());
    glm::vec3 Right = glm::normalize(glm::cross(Front, Up));
    Up = glm::normalize(glm::cross(Right, Front));
#endif  
#ifdef  World_Oriented_Billboard
    glm::vec3  Up = glm::vec3(0, 1, 0);
    glm::vec3 Front = glm::normalize(-Camera::getMainCameraFront());
    glm::vec3 Right = glm::normalize(glm::cross( Front, Up));
    Up = glm::normalize(glm::cross( Right,Front));

#endif  
#ifdef    Y_Axial_Billboard
    glm::vec3  Up = glm::vec3(0, 1, 0);
    glm::vec3 Front = glm::normalize(-Camera::getMainCameraFront());
    glm::vec3 Right = glm::normalize(glm::cross(Front, Up));
    Front = glm::normalize(glm::cross(Up,Right));
#endif

    BillBoardRotate[0] = glm::vec4(Right, 0.0);
    BillBoardRotate[1] = glm::vec4(Up, 0.0);
    BillBoardRotate[2] = glm::vec4(Front, 0.0);

    m_pRenderQuadShader->setMat4UniformValue("billBoardRotation", glm::value_ptr(BillBoardRotate));

    glDrawArrays(GL_TRIANGLES, 0,6);

    glBindVertexArray(0u);
    glUseProgram(0u);
}
 