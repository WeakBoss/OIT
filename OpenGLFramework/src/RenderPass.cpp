#include "RenderPass.h"
#include "Shader.h"

IRenderPass::IRenderPass()
{
}

IRenderPass::IRenderPass(const std::string& vPassName, int vExecutionOrder):m_PassName(vPassName),m_ExecutionOrder(vExecutionOrder)
{
}

IRenderPass::~IRenderPass()
{
}

//************************************************************************************
//Function:
bool IRenderPass::operator<(const IRenderPass& vOtherPass) const
{
 	return m_ExecutionOrder < vOtherPass.getExecutionOrder();
}