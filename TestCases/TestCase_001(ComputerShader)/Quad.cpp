#include "pch.h"
#include "Quad.h"


//**************************************************************************************************
//FUNCTION:
void CQuadObj::initV()
{
    GLfloat vertices[] = {
         -0.5f, -0.5f, -0.0f,
         0.5f, -0.5f, -0.0f,
         0.5f,  0.5f, -0.0f,
         0.5f,  0.5f, -0.0f,
         -0.5f,  0.5f, -0.0f,
         -0.5f, -0.5f, -0.0f,
    };
    m_VAO = createVAO(vertices, sizeof(vertices), { 3 });

 

}
//**************************************************************************************************
//FUNCTION:
void CQuadObj::updateV()
{

}
