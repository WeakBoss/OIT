#pragma once
#include "pch.h"
#include "Quad.h"
class CBillBoardRenderPass : public IRenderPass
{

public:
    CBillBoardRenderPass(const std::string& vPassName,int vExecutionOrder):IRenderPass(vPassName,vExecutionOrder){}
    ~CBillBoardRenderPass() 
    {
 
    }

    void initV() override;
    void updateV() override;
private:
    std::shared_ptr<CQuadObj> m_pQuadObj = nullptr;
    std::shared_ptr<CShader> m_pRenderQuadShader = nullptr;
};
