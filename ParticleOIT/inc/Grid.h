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
    float getScaleFactor() const { return m_scaleFactor; }
    void setScaleFactor(float vVal)
    {
        m_scaleFactor = vVal; 
        setScale(glm::vec3(m_scaleFactor));
    }
private:
    GLuint m_VAO;
    float m_scaleFactor;
};