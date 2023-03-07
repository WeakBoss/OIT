#include "ParticleOITPCH.h"
#include "ParticleSystemObj.h"
#include "DirectRenderPass.h"
int main()
{
    
    {
        WINDOW_KEYWORD::setIsCursorDisable(false);
        WINDOW_KEYWORD::setWindowSize(1200, 1200);

        COMPONENT_CONFIG::setIsEnableGUI(true);

        ResourceManager::registerGameObject(std::make_shared<CParticleSystemObj>("ParticleSystem",1));
        ResourceManager::registerRenderPass(std::make_shared<CDirectRenderPass>("DirectRenderPass", 1));
        App::initApp();
        App::updateApp();
    }
    return 0;
}