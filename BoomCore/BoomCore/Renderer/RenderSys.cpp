#include "RenderSys.h"

#include "Camera/FPSCamera.h"
#include "Camera/TPSCamera.h"

namespace Kawaii
{
	void RenderSys::initialize(int width, int height)
	{
		m_width = width;
		m_height = height;
		
		m_meshMgr = MeshMgr::getSingleton();
		m_shaderMgr = ShaderMgr::getSingleton();
		m_textureMgr = TextureMgr::getSingleton();
		m_renderList = std::make_shared<RenderTargetList>();

	}

	void RenderSys::setSkyDome(const std::string& path, const std::string& pFix)
	{
		if (m_skyDome != nullptr)
			return;

		unsigned int skyboxShader = m_shaderMgr->loadShader("skybox", 
			"Shaders/skybox.vs", "Shaders/skybox.fs");
		unsigned int cubeTex = m_textureMgr->loadTextureCube("skybox", path, pFix);
		unsigned int meshIndex = m_meshMgr->loadMesh(new Sphere(1.0f, 10, 10));
		//passing all infomation to Skydome
		m_skyDome = std::make_shared<SkyDome>(skyboxShader);
		m_skyDome->addMesh(meshIndex);
		m_skyDome->addTexture(cubeTex);
	}

	Camera3D::ptr RenderSys::createCamera(glm::vec3 pos, glm::vec3 target)
	{
		FPSCamera* cam_ = new FPSCamera(pos);
		cam_->lookAt(glm::normalize(target - pos), Camera3D::LocalUp);
		m_camera = std::shared_ptr<Camera3D>(cam_);
		return m_camera;
	}

	void RenderSys::render()
	{
		if (m_renderList == nullptr) return;

		//need add depth test, clear color etc

		m_renderList->render(m_camera, m_sunLight, m_lightCamera);

	}

}