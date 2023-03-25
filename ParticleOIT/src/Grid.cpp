#include "ParticleOITPCH.h"
#include "Grid.h"

//**************************************************************************************************
//FUNCTION:
void CGridObj::initV()
{
    GLfloat QuadVertices[] = {
        1.0f, 1.0f,0.0f,  1.0f,1.0f,
        1.0f,-1.0f,0.0f,   1.0f,0.0f,
        -1.0f, 1.0f,0.0f,   0.0f,1.0f,
        -1.0f, 1.0f,0.0f,   0.0f,1.0f,
        1.0f,-1.0f, 0.0f,  1.0f,0.0f,
        -1.0f,-1.0f,0.0f,   0.0f,0.0f
    };
    m_VAO = createVAO(QuadVertices, sizeof(QuadVertices), { 3,2 });
    m_scaleFactor = 200.0f;
    setScale(glm::vec3(m_scaleFactor));
    setRotationX(90.0f);
}
//**************************************************************************************************
//FUNCTION:
void CGridObj::updateV()
{

}
