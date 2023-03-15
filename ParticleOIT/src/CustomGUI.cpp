#include "ParticleOITPCH.h"
#include "CustomGUI.h"


//**************************************************************************************************
//FUNCTION:
void CCustomGUI::initV() //比Obj先init，但是obj已经构造了
{
	m_pParticleSystemObj = std::dynamic_pointer_cast<CParticleSystemObj>(ResourceManager::getGameObjectByName("ParticleSystem"));
	m_pGridObj = std::dynamic_pointer_cast<CGridObj>(ResourceManager::getGameObjectByName("Grid"));

	//m_SimulationParametersSet
	m_SimulationParametersSet.resize(3);
	m_SimulationParametersSet[1].emitter_position = glm::vec4(50.0f, 0.0f, 0.0f, 0.0f);
	m_SimulationParametersSet[2].emitter_position = glm::vec4(50.0f, -50.0f, 0.0f, 0.0f);

	m_pParticleSystemObj->addParticleType(m_SimulationParametersSet);
	

	ResourceManager::registerSharedData("DirectRenderSimulationParameters", m_RenderingParameters);
}

//**************************************************************************************************
//FUNCTION:
void CCustomGUI::updateV()
{
	IGUI::pushItemWidth(WINDOW_KEYWORD::getWindowWidth() * 0.05f);

	IGUI::text(std::string("Alive Particles: ") + std::to_string(m_pParticleSystemObj->getNumAliveParticles()));

	IGUI::text("Simulation: ");
 
	if (IGUI::sliderFloat("time step factor", m_SimulationParametersSet[0].time_step_factor, 0.1, 10));
		m_pParticleSystemObj->setSimulationParameters(m_SimulationParametersSet[0],0);
	
	IGUI::horizontalLine();
 
	IGUI::text("Render: ");
	if (IGUI::sliderFloat("MinSize", m_RenderingParameters.min_size, 1, 50));
	//ResourceManager::updateSharedDataByName("DirectRenderSimulationParameters", m_RenderingParameters);

 
	IGUI::popItemWidth();
}
