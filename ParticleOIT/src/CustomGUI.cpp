#include "ParticleOITPCH.h"
#include "CustomGUI.h"


//**************************************************************************************************
//FUNCTION:
void CCustomGUI::initV()
{
	m_pParticleSystemObj = std::dynamic_pointer_cast<CParticleSystemObj>(ResourceManager::getGameObjectByName("ParticleSystem"));
	m_pGridObj = std::dynamic_pointer_cast<CGridObj>(ResourceManager::getGameObjectByName("Grid"));

}

//**************************************************************************************************
//FUNCTION:
void CCustomGUI::updateV()
{
	IGUI::text("Transfer Func: ");
	IGUI::horizontalLine();
	IGUI::pushItemWidth(WINDOW_KEYWORD::getWindowWidth() * 0.05f);
 
		
	float MinTimeFactor = 0.1f, MaxTimeFactor = 10.0f;
	if(IGUI::sliderScalar("time step factor", EDataType::DataType_Float, &m_SimulationParameters.time_step_factor, &MinTimeFactor, &MaxTimeFactor, "%.1f"))
		m_pParticleSystemObj->setSimulationParameters(m_SimulationParameters);
		
	float MinVolumeSize = 16.0f, MaxVolumeSize = 500.0f;
	if(IGUI::sliderScalar("Volume Size", EDataType::DataType_Float, &m_SimulationParameters.bounding_volume_size, &MinVolumeSize, &MaxVolumeSize, "%.0f"))
		m_pParticleSystemObj->setSimulationParameters(m_SimulationParameters);



 
	IGUI::popItemWidth();
}
