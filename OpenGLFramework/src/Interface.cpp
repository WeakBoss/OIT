#include "Interface.h"
#include "App.h"
#include "Common.h"
#include "Camera.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "UBO4ProjectionWorld.h"

//************************************************************************************
//Function:
void hiveGraphics::App::initApp()
{
	CApp::getOrCreateInstance()->init();
}

//************************************************************************************
//Function:
void hiveGraphics::App::updateApp()
{
	CApp::getOrCreateInstance()->update();
}

//************************************************************************************
//Function:
double hiveGraphics::App::getDeltaTime()
{
	return CApp::getOrCreateInstance()->getDeltaTime();
}

//************************************************************************************
//Function:
double hiveGraphics::App::getCurrentTime()
{
	return CApp::getOrCreateInstance()->getCurrentTime();
}

//************************************************************************************
//Function:
double hiveGraphics::App::getFrameRateInMilliSecond()
{
	return CApp::getOrCreateInstance()->getFrameRateInMilliSecond();
}

//************************************************************************************
//Function:
int hiveGraphics::App::getFramesPerSecond()
{
	return CApp::getOrCreateInstance()->getFramesPerSecond();
}

//************************************************************************************
//Function:
int hiveGraphics::WINDOW_KEYWORD::getWindowWidth()
{
	return WINDOW_WIDTH;
}

//************************************************************************************
//Function:
int hiveGraphics::WINDOW_KEYWORD::getWindowHeight()
{
	return WINDOW_HEIGHT;
}

//************************************************************************************
//Function:
void hiveGraphics::WINDOW_KEYWORD::setWindowSize(int vWidth, int vHeight, bool vIsViewportSizeChangedWithWindow/* = true*/)
{
	WINDOW_WIDTH = vWidth;
	WINDOW_HEIGHT = vHeight;
	if (vIsViewportSizeChangedWithWindow)
	{
		VIEWPORT_WIDTH = vWidth;
		VIEWPORT_HEIGHT = vHeight;
	}
}

//************************************************************************************
//Function:
void hiveGraphics::WINDOW_KEYWORD::setViewportSize(int vWidth, int vHeight)
{
	VIEWPORT_WIDTH = vWidth;
	VIEWPORT_HEIGHT = vHeight;
}

//************************************************************************************
//Function:
void hiveGraphics::WINDOW_KEYWORD::setSampleNum(int vSampleNum)
{
	NUM_SAMPLES = vSampleNum;
}

//************************************************************************************
//Function:
void hiveGraphics::WINDOW_KEYWORD::setIsCursorDisable(bool vIsCursorDisable)
{
	CURSOR_DISABLE = vIsCursorDisable;
}

//************************************************************************************
//Function:
void hiveGraphics::WINDOW_KEYWORD::setWindowTile(const std::string& vWindowTitle)
{
	WINDOW_TITLE = vWindowTitle;
}

//************************************************************************************
//Function:
void hiveGraphics::COMPONENT_CONFIG::setIsEnableGUI(bool vIsEnableGUI)
{
	IS_ENABLE_GUI = vIsEnableGUI;
}

//************************************************************************************
//Function:
const glm::dvec3& hiveGraphics::Camera::getMainCameraPos()
{
	return CResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getCameraPos();
}

//************************************************************************************
//Function:
glm::dvec3 hiveGraphics::Camera::getMainCameraLookAtPos()
{
	return CResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getLookAtPos();
}
//************************************************************************************
//Function:
const glm::dvec3& hiveGraphics::Camera::getMainCameraFront()
{
	return CResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getCameraFront();
}

//************************************************************************************
//Function:
const glm::dvec3& hiveGraphics::Camera::getMainCameraUp()
{
	return CResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getCameraUp();
}

//************************************************************************************
//Function:
double hiveGraphics::Camera::getMainCameraFov()
{
	return CResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getCameraFov();
}

//************************************************************************************
//Function:
double hiveGraphics::Camera::getMainCameraNear()
{
	return CResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getCameraNear();
}

//************************************************************************************
//Function:
double hiveGraphics::Camera::getMainCameraFar()
{
	return CResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getCameraFar();
}

//************************************************************************************
//Function:
glm::mat4 hiveGraphics::Camera::getMainCameraProjectionMatrix()
{
	return CResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getProjectionMatrix();
}

//************************************************************************************
//Function:
glm::mat4 hiveGraphics::Camera::getMainCameraViewMatrix()
{
	return CResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getViewMatrix();
}

//************************************************************************************
//Function:
glm::mat4 hiveGraphics::Camera::getMainCameraPrevViewMatrix()
{
	return CResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getPrevViewMatrix();
}

//************************************************************************************
//Function:
void hiveGraphics::Camera::setMainCameraPos(glm::dvec3 vCameraPos)
{
	return CResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->setCameraPos(vCameraPos);
}

//************************************************************************************
//Function:
void hiveGraphics::Camera::setMainCameraFront(glm::dvec3 vCameraFront)
{
	return CResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->setCameraFront(vCameraFront);
}

//************************************************************************************
//Function:
void hiveGraphics::Camera::setMainCameraFarPlane(double vFarPlane)
{
	auto pMainCamera = CResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera();
	pMainCamera->setFarPlane(vFarPlane);
	CResourceManager::getOrCreateInstance()->fetchOrCreateUBO4ProjectionWorld()->updateProjectionMatrix(pMainCamera->getProjectionMatrix());
}

//************************************************************************************
//Function:
void hiveGraphics::Camera::setMainCameraMoveSpeed(double vMoveSpeed)
{
	CResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->setMoveSpeed(vMoveSpeed);
}

//************************************************************************************
//Function:
void hiveGraphics::Camera::setMainCameraFov(double vFov)
{
	auto pMainCamera = CResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera();
	pMainCamera->setFov(vFov);
	CResourceManager::getOrCreateInstance()->fetchOrCreateUBO4ProjectionWorld()->updateProjectionMatrix(pMainCamera->getProjectionMatrix());
}

//************************************************************************************
//Function:
void hiveGraphics::Camera::setIsEnableCursor(bool vIsEnableCursor)
{
	CResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->setEnableCursor(vIsEnableCursor);
}

//************************************************************************************
//Function:
const std::shared_ptr<IGameObject>& hiveGraphics::ResourceManager::getGameObjectByName(const std::string &vGameObjectName)
{
	return CResourceManager::getOrCreateInstance()->getGameObjectByName(vGameObjectName);
}

//************************************************************************************
//Function:
void hiveGraphics::ResourceManager::registerSharedData(const std::string& vDataName, boost::any vData)
{
	CResourceManager::getOrCreateInstance()->registerSharedData(vDataName, vData);
}

//************************************************************************************
//Function:
void hiveGraphics::ResourceManager::registerRenderPass(const std::shared_ptr<IRenderPass> &vRenderPass)
{
	CResourceManager::getOrCreateInstance()->registerRenderPass(vRenderPass);
}

//************************************************************************************
//Function:
void hiveGraphics::ResourceManager::registerGameObject(const std::shared_ptr<IGameObject> &vGameObject)
{
	CResourceManager::getOrCreateInstance()->registerGameObject(vGameObject);
}

//************************************************************************************
//Function:
void hiveGraphics::ResourceManager::registerSubGUI(const std::shared_ptr<IGUI> &vSubGUI)
{
	CResourceManager::getOrCreateInstance()->registerSubGUI(vSubGUI);
}

//************************************************************************************
//Function:
const std::shared_ptr<CModel>& hiveGraphics::ResourceManager::getOrCreateModel(const std::string &vModelPath) 
{
	return CResourceManager::getOrCreateInstance()->getOrCreateModel(vModelPath);
}

//************************************************************************************
//Function:
GLint hiveGraphics::ResourceManager::getOrCreateScreenQuadVAO()
{
	return CResourceManager::getOrCreateInstance()->getOrCreateScreenQuadVAO();
}

//************************************************************************************
//Function:
GLint hiveGraphics::ResourceManager::getOrCreateCubeVAO()
{
	return CResourceManager::getOrCreateInstance()->getOrCreateCubeVAO();
}

//************************************************************************************
//Function:
const std::shared_ptr<CMainGUI>& hiveGraphics::ResourceManager::getOrCreateMainGUI()
{
	return CResourceManager::getOrCreateInstance()->getOrCreateMainGUI();
}

//************************************************************************************
//Function:
const boost::any& hiveGraphics::ResourceManager::getSharedDataByName(const std::string &vDataName)
{
	return CResourceManager::getOrCreateInstance()->getSharedDataByName(vDataName);
}

//************************************************************************************
//Function:
void hiveGraphics::ResourceManager::updateSharedDataByName(const std::string& vDataName, const boost::any& vData)
{
	CResourceManager::getOrCreateInstance()->updateSharedDataByName(vDataName, vData);
}

//************************************************************************************
//Function:
int hiveGraphics::InputManager::getKeyStatus(int vKey)
{
	return CInputManager::getOrCreateInstance()->getKeyStatus()[vKey];
}