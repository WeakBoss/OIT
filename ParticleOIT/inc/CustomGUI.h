#pragma once
#include "ParticleOITPCH.h"
#include "ParticleSystemObj.h"
#include "Grid.h"

class CCustomGUI : public IGUI
{
public:
	CCustomGUI(const std::string& vName, int vExecutionOrder) : IGUI(vName, vExecutionOrder) {}
	~CCustomGUI() = default;

	void initV() override;
	void updateV() override;

private:
	std::shared_ptr<CParticleSystemObj> m_pParticleSystemObj = nullptr;
	std::shared_ptr<CGridObj> m_pGridObj = nullptr;

	std::vector<SSimulationParameters> m_SimulationParametersSet;
	std::vector<SRenderParameters> m_RenderingParametersSet;

	void readParametersFromFile();
	const std::string m_ParametersPath = "src/Parameters.json";
 
	 
};