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

        Camera::setMainCameraPos(glm::vec3(0.0f, 20.0f, 50.0f));
        Camera::setMainCameraFov(60.0f);
        Camera::setMainCameraFarPlane(2000.0f);
        Camera::setMainCameraFront(glm::vec3(0.0f, -0.5f, -1.0f));
        Camera::setMainCameraMoveSpeed(20.0);

        App::updateApp();
    }
    return 0;
}