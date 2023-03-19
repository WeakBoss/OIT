#include "ParticleOITPCH.h"
#include "CustomGUI.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <magic_enum.hpp>
#include <fmt/core.h>
//**************************************************************************************************
//FUNCTION:
void CCustomGUI::initV() //比Obj先init，但是obj已经构造了
{
	m_pParticleSystemObj = std::dynamic_pointer_cast<CParticleSystemObj>(ResourceManager::getGameObjectByName("ParticleSystem"));
	m_pGridObj = std::dynamic_pointer_cast<CGridObj>(ResourceManager::getGameObjectByName("Grid"));

    readParametersFromFile();

	ResourceManager::registerSharedData("SimulationParametersSet", m_SimulationParametersSet);
	m_pParticleSystemObj->addParticleType(m_SimulationParametersSet);

	ResourceManager::registerSharedData("DirectRenderSimulationParameters", m_RenderingParametersSet);


}

//**************************************************************************************************
//FUNCTION:
void CCustomGUI::updateV()
{
	IGUI::pushItemWidth(WINDOW_KEYWORD::getWindowWidth() * 0.05f);

	IGUI::text(std::string("Alive Particles: ") + std::to_string(m_pParticleSystemObj->getNumAliveParticles()));
 
    //collapsingHeader
    if (IGUI::collapsingHeader("Simulation:"))
    {
        for (int i = 0; i < m_SimulationParametersSet.size(); i++)
        {
            if (IGUI::collapsingHeader(fmt::format("Type{}:",i)))
            {
                IGUI::sliderFloat("time step factor", m_SimulationParametersSet[i].time_step_factor, 0.1, 10);
            }
        }
    }
    ResourceManager::updateSharedDataByName("SimulationParametersSet", m_SimulationParametersSet);

	IGUI::horizontalLine();
    if (IGUI::collapsingHeader("Render:"))
    {

    }

	IGUI::text("Render: ");
	//if (IGUI::sliderFloat("MinSize", m_RenderingParameters.min_size, 1, 50));
	////ResourceManager::updateSharedDataByName("DirectRenderSimulationParameters", m_RenderingParameters);

 
	IGUI::popItemWidth();
}

inline glm::vec4 getVec4FromKey(boost::property_tree::ptree::value_type& vPTree,const std::string& vKey)
{
    std::vector<float> TempFloatVec;
    for (boost::property_tree::ptree::value_type& FloatValue : vPTree.second.get_child(vKey))
        TempFloatVec.push_back(FloatValue.second.get_value<float>());
    return glm::vec4(TempFloatVec[0], TempFloatVec[1], TempFloatVec[2], TempFloatVec[3]);
}
//**************************************************************************************************
//FUNCTION:
void CCustomGUI::readParametersFromFile()
{
    // 读取JSON配置文件
    boost::property_tree::ptree PTree;
    boost::property_tree::read_json(m_ParametersPath, PTree);

    // 获取simulation_parameters数组节点
    boost::property_tree::ptree& SimulationParametersArray = PTree.get_child("simulation_parameters");
    for (boost::property_tree::ptree::value_type& SimulationParametersNode : SimulationParametersArray)
    {
        SSimulationParameters SimulationParameters;

        SimulationParameters.emitter_position = getVec4FromKey(SimulationParametersNode, "emitter_position");
        SimulationParameters.emitter_direction = getVec4FromKey(SimulationParametersNode, "emitter_direction");
        SimulationParameters.emit_num_per_second = SimulationParametersNode.second.get<unsigned int>("emit_num_per_second");
        SimulationParameters.time_step_factor = SimulationParametersNode.second.get<float>("time_step_factor");
        SimulationParameters.min_age = SimulationParametersNode.second.get<float>("min_age");
        SimulationParameters.max_age = SimulationParametersNode.second.get<float>("max_age");
        auto Type = magic_enum::enum_cast<EmitterType>(SimulationParametersNode.second.get<std::string>("emitter_type"));
        SimulationParameters.emitter_type = Type.value();
        SimulationParameters.emitter_radius = SimulationParametersNode.second.get<float>("emitter_radius");
        SimulationParameters.scattering_factor = SimulationParametersNode.second.get<float>("scattering_factor");
        SimulationParameters.vectorfield_factor = SimulationParametersNode.second.get<float>("vectorfield_factor");
        SimulationParameters.curlnoise_factor = SimulationParametersNode.second.get<float>("curlnoise_factor");
        SimulationParameters.curlnoise_scale = SimulationParametersNode.second.get<float>("curlnoise_scale");
        SimulationParameters.velocity_factor = SimulationParametersNode.second.get<float>("velocity_factor");
        SimulationParameters.padding = SimulationParametersNode.second.get<float>("padding");
        SimulationParameters.enable_scattering = SimulationParametersNode.second.get<unsigned int>("enable_scattering");
        SimulationParameters.enable_vectorfield = SimulationParametersNode.second.get<unsigned int>("enable_vectorfield");
        SimulationParameters.enable_curlnoise = SimulationParametersNode.second.get<unsigned int>("enable_curlnoise");
        SimulationParameters.enable_velocity_control = SimulationParametersNode.second.get<unsigned int>("enable_velocity_control");

        m_SimulationParametersSet.emplace_back(std::move(SimulationParameters));
    }
    boost::property_tree::ptree& RenderParametersArray = PTree.get_child("render_parameters");
    for (const auto& RenderParametersNode : RenderParametersArray)
    {
        SRenderParameters RenderParameters;
        RenderParameters.min_size = RenderParametersNode.second.get<float>("min_size");
        RenderParameters.max_size = RenderParametersNode.second.get<float>("max_size");
        RenderParameters.fading_factor = RenderParametersNode.second.get<float>("fading_factor");

        m_RenderingParametersSet.emplace_back(std::move(RenderParameters));
    } 

}
