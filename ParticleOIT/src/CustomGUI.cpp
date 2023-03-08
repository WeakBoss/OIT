#include "ParticleOITPCH.h"
#include "CustomGUI.h"


//**************************************************************************************************
//FUNCTION:
void CCustomGUI::initV()
{
	m_pParticleSystemObj = std::dynamic_pointer_cast<CParticleSystemObj>(ResourceManager::getGameObjectByName("ParticleSystem"));
	m_pGridObj = std::dynamic_pointer_cast<CGridObj>(ResourceManager::getGameObjectByName("Grid"));

	ResourceManager::registerSharedData("DirectRenderSimulationParameters", m_RenderingParameters);
}

//**************************************************************************************************
//FUNCTION:
void CCustomGUI::updateV()
{
	IGUI::pushItemWidth(WINDOW_KEYWORD::getWindowWidth() * 0.05f);

	IGUI::text("Simulation: ");
	if (IGUI::sliderFloat("time step factor", m_SimulationParameters.time_step_factor, 0.1, 10));
		m_pParticleSystemObj->setSimulationParameters(m_SimulationParameters);
	
	IGUI::horizontalLine();
 
	IGUI::text("Render: ");
	if (IGUI::sliderFloat("MinSize", m_RenderingParameters.min_size, 1, 50));
	ResourceManager::updateSharedDataByName("DirectRenderSimulationParameters", m_RenderingParameters);

 
	IGUI::popItemWidth();
}
