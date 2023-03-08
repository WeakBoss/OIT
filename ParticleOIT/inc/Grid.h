#pragma once
#include "ParticleOITPCH.h"


class CGridObj : public IGameObject
{
public:
    CGridObj(const std::string& vGameObjectName, int vExecutionOrder) :IGameObject(vGameObjectName, vExecutionOrder)
    {
        
    }
    ~CGridObj() = default;
    void initV() override;
    void updateV() override;
    void use()const { glBindVertexArray(m_VAO); }
    GLsizei getNumVertices()const { return m_NumVertices; }
private:
    GLuint m_VAO;
    GLuint m_VBO;
    GLsizei m_NumVertices;
    unsigned int m_Resolution;
};