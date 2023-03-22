#include "pch.h"
#include "Quad.h"
#include "BillBoardRenderPass.h"
 
int main()
{
    {
        WINDOW_KEYWORD::setIsCursorDisable(false);
        WINDOW_KEYWORD::setWindowSize(1536, 864);

        COMPONENT_CONFIG::setIsEnableGUI(true);

        ResourceManager::registerGameObject(std::make_shared<CQuadObj>("Quad", 1));

        ResourceManager::registerRenderPass(std::make_shared<CBillBoardRenderPass>("BillBoardRenderPass", 1));

        App::initApp();

        Camera::setMainCameraPos(glm::vec3(0.0f, 0.0f, 10.0f));
        Camera::setMainCameraFov(60.0f);
        Camera::setMainCameraFarPlane(2000.0f);
        Camera::setMainCameraFront(glm::vec3(0.0f, 0.0f, -1.0f));
        Camera::setMainCameraMoveSpeed(20.0);

        App::updateApp();
    }
    return 0;
}