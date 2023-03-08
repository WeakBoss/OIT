#include "ParticleOITPCH.h"
#include "ParticleSystemObj.h"
#include "DirectRenderPass.h"
#include "Grid.h"
#include "CustomGUI.h"
int main()
{
    {
        WINDOW_KEYWORD::setIsCursorDisable(false);
        WINDOW_KEYWORD::setWindowSize(1536, 864);

        COMPONENT_CONFIG::setIsEnableGUI(true);

        ResourceManager::registerGameObject(std::make_shared<CGridObj>("Grid", 1));
        ResourceManager::registerGameObject(std::make_shared<CParticleSystemObj>("ParticleSystem",2));

        ResourceManager::registerRenderPass(std::make_shared<CDirectRenderPass>("DirectRenderPass", 1));

        ResourceManager::registerSubGUI(std::make_shared<CCustomGUI>("CustomGui", 1));
        App::initApp();
        App::updateApp();
    }
    return 0;
}