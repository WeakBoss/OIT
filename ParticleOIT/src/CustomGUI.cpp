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
	 

	IGUI::text(std::string("Alive Particles: ") + std::to_string(m_pParticleSystemObj->getNumAliveParticles()));
 
    //collapsingHeader
    if (IGUI::collapsingHeader("Simulation:"))
    {
        for (int i = 0; i < m_SimulationParametersSet.size(); i++)
        {
            
            if (IGUI::treeNode(fmt::format("Type{}:",i)))
            {
                uint32_t MinEmitNum = 100u; uint32_t MaxEmitNum = 5000u;
                IGUI::sliderScalar("emit num per second", EDataType::DataType_U32, &m_SimulationParametersSet[i].emit_num_per_second, &MinEmitNum, &MaxEmitNum, "%u");
                IGUI::sliderFloat("time step factor", m_SimulationParametersSet[i].time_step_factor, 0.01f, 20.0f);
                IGUI::sliderFloat("min_age", m_SimulationParametersSet[i].min_age, 0.0f, 100.0f);
                IGUI::sliderFloat("max_age", m_SimulationParametersSet[i].max_age, 100.0f, 1000.0f);
                IGUI::sliderFloat("emitter_radius", m_SimulationParametersSet[i].emitter_radius, 0.01f, 10.0f);
                
                {
                    /*bool enable_vectorfield = m_SimulationParametersSet[i].enable_vectorfield;
                    IGUI::checkBox("enable_vectorfield", enable_vectorfield);
                    m_SimulationParametersSet[i].enable_vectorfield = enable_vectorfield;
                    IGUI::sameLine();
                    IGUI::sliderFloat("factor##enable_vectorfield", m_SimulationParametersSet[i].vectorfield_factor, 0.01f, 1.0f);*/
                }
                {
                    bool enable_scattering = m_SimulationParametersSet[i].enable_scattering;
                    IGUI::checkBox("scattering", enable_scattering);
                    m_SimulationParametersSet[i].enable_scattering = enable_scattering;
                    IGUI::sameLine();
                    IGUI::sliderFloat("factor##scattering", m_SimulationParametersSet[i].scattering_factor, 0.01f, 1.0f);
                }
                {
                    /* bool enable_velocity_control = m_SimulationParametersSet[i].enable_velocity_control;
                     IGUI::checkBox("enable_velocity_control", enable_velocity_control);
                     m_SimulationParametersSet[i].enable_velocity_control = enable_velocity_control;
                     IGUI::sameLine();
                     IGUI::sliderFloat("factor##enable_velocity_control", m_SimulationParametersSet[i].velocity_factor, 0.1f, 10.0f);*/
                }
                {
                    //bool enable_curlnoise = m_SimulationParametersSet[i].enable_curlnoise;
                    //IGUI::checkBox("enable_curlnoise", enable_curlnoise);
                    //m_SimulationParametersSet[i].enable_curlnoise = enable_curlnoise;
                    //IGUI::sameLine();
                    //IGUI::sliderFloat("factor##enable_curlnoise", m_SimulationParametersSet[i].curlnoise_factor, 0.01f, 1.0f);
                    //IGUI::sliderFloat("scale", m_SimulationParametersSet[i].curlnoise_scale, 1.0f, 200.0f);
                }



                IGUI::treePop();
            }
           
            IGUI::separator();
            
        }
    }
    ResourceManager::updateSharedDataByName("SimulationParametersSet", m_SimulationParametersSet);

	IGUI::horizontalLine();
    if (IGUI::collapsingHeader("Render:"))
    {
        for (int i = 0; i < m_RenderingParametersSet.size(); i++)
        {
            if (IGUI::treeNode(fmt::format("Type{}:", i)))
            {
                IGUI::sliderFloat("max_size", m_RenderingParametersSet[i].max_size, 1.0f, 100.0f);
                IGUI::sliderFloat("size_scale_factor", m_RenderingParametersSet[i].size_scale_factor, 0.1f, 5.0f);
                IGUI::colorEdit("color", m_RenderingParametersSet[i].particle_color, false);
                IGUI::sliderFloat("alpha_factor", m_RenderingParametersSet[i].alpha_factor, 0.001f,0.3f);
                IGUI::treePop();
            }
            IGUI::separator();
        }
    }

 
	//if (IGUI::sliderFloat("MinSize", m_RenderingParameters.min_size, 1, 50));
	ResourceManager::updateSharedDataByName("DirectRenderSimulationParameters", m_RenderingParametersSet);

 
 
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
    for (boost::property_tree::ptree::value_type& RenderParametersNode : RenderParametersArray)
    {
        SRenderParameters RenderParameters;
        RenderParameters.min_size = RenderParametersNode.second.get<float>("min_size");
        RenderParameters.max_size = RenderParametersNode.second.get<float>("max_size");
        RenderParameters.size_scale_factor = RenderParametersNode.second.get<float>("size_scale_factor");
        RenderParameters.alpha_factor = RenderParametersNode.second.get<float>("alpha_factor");
        RenderParameters.particle_color = getVec4FromKey(RenderParametersNode, "particle_color");

        m_RenderingParametersSet.emplace_back(std::move(RenderParameters));
    } 

}
