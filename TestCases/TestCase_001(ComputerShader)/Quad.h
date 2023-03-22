#pragma once
#include "pch.h"


class CQuadObj : public IGameObject
{
public:
    CQuadObj(const std::string& vGameObjectName, int vExecutionOrder) :IGameObject(vGameObjectName, vExecutionOrder)
    {
        
    }
    ~CQuadObj() = default;
    void initV() override;
    void updateV() override;
    void use()const { glBindVertexArray(m_VAO); }
     
private:
    float m_ScaleFactor = 1024.0f;
    GLuint m_VAO;
};