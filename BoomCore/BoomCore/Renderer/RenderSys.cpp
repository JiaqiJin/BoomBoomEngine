#include "RenderSys.h"

#include "Camera/FPSCamera.h"
#include "Camera/TPSCamera.h"

#include "PostProcess/IBL.h"

namespace Kawaii
{
	void RenderSys::resize(int width, int height)
	{
		m_width = width;
		m_height = height;
		// resize event.
		glViewport(0, 0, width, height);
		if (m_camera != nullptr)
			m_camera->changeAspect(static_cast<float>(width) / height);
	}

	void RenderSys::initialize(int width, int height)
	{
		m_width = width;
		m_height = height;
		// initialization.
		resize(width, height);
		m_meshMgr = MeshMgr::getSingleton();
		m_shaderMgr = ShaderMgr::getSingleton();
		m_textureMgr = TextureMgr::getSingleton();
		m_renderList = std::make_shared<RenderTargetList>();
		
	}

	void RenderSys::setSkyDomeHdr(const std::string& path)
	{
		if (m_skyDome != nullptr)
			return;
		unsigned int skyboxShader = m_shaderMgr->loadShader("skybox",
			"Shaders/skybox.vs", "Shaders/skybox.fs");
		unsigned int hdrTexIndex = m_textureMgr->loadTexture2DHdr("hdrTex", path);
		unsigned int cubeTexIndex = m_textureMgr->loadTextureCubeHdrRaw("skyboxCubemap", nullptr, 1024, 1024);

		//convert hdrmap to cubemap
		IBLAux::convertToCubemap(1024, 1024, hdrTexIndex, cubeTexIndex);

		unsigned int mesh = m_meshMgr->loadMesh(new Sphere(1.0f, 10, 10));
		m_skyDome = std::make_shared<SkyDome>(skyboxShader);
		PBRMaterial mat;
		mat.m_albedoTexIndex = cubeTexIndex;
		m_skyDome->addMesh(mesh);
		m_skyDome->addPbrTexture(mat);
	}

	void RenderSys::setSkyDome(const std::string& path, const std::string& pFix)
	{
		// set skydome background.
		if (m_skyDome != nullptr)
			return;
		unsigned int skyboxShader = m_shaderMgr->loadShader("skybox",
			"Shaders/skybox.vs", "Shaders/skybox.fs");
		unsigned int cubeTex = m_textureMgr->loadTextureCube("skybox", path, pFix);
		unsigned int mesh = m_meshMgr->loadMesh(new Sphere(1.0f, 10, 10));
		m_skyDome = std::make_shared<SkyDome>(skyboxShader);
		m_skyDome->addMesh(mesh);
		m_skyDome->addTexture(cubeTex);
	}

	Camera3D::ptr RenderSys::createFPSCamera(glm::vec3 pos, glm::vec3 target)
	{
		// create a first person camera.
		FPSCamera* _cam = new FPSCamera(pos);
		_cam->lookAt(glm::normalize(target - pos), Camera3D::LocalUp);
		m_camera = std::shared_ptr<Camera3D>(_cam);
		return m_camera;
	}

	Camera3D::ptr RenderSys::createTPSCamera(glm::vec3 pos, glm::vec3 target)
	{
		// create a third person camera.
		TPSCamera* _cam = new TPSCamera(target, 0.0f, 30.0f, 3.0f);
		m_camera = std::shared_ptr<Camera3D>(_cam);
		return m_camera;
	}

	void RenderSys::createSunLightCamera(glm::vec3 target, float left, float right,
		float bottom, float top, float near, float far)
	{
		if (m_sunLight == nullptr)
		{
			std::cout << "You haven't create a light source.\n";
			return;
		}
		const float length = 200.0f;
		glm::vec3 pos = length * m_sunLight->getDirection();
		if (m_lightCamera == nullptr)
		{
			FPSCamera* cam = new FPSCamera(pos);
			m_lightCamera = std::shared_ptr<Camera3D>(cam);
		}
		m_lightCamera->setOrthographicProject(left, right, bottom, top, near, far);
		FPSCamera* cam = reinterpret_cast<FPSCamera*>(m_lightCamera.get());
		cam->lookAt(-m_sunLight->getDirection(), Camera3D::LocalUp);
	}
	/*
	void RenderSys::createPointLightCamera(glm::vec3 target, float left, float right,
		float bottom, float top, float near, float far)
	{
		if (m_pointLight == nullptr)
		{
			std::cout << "You haven't create a light source.\n";
			return;
		}
		const float length = 200.0f;
		glm::vec3 pos = length * m_pointLight->getPosition();
		if (m_lightCamera == nullptr)
		{
			FPSCamera* cam = new FPSCamera(pos);
			m_lightCamera = std::shared_ptr<Camera3D>(cam);
		}
		m_lightCamera->setOrthographicProject(left, right, bottom, top, near, far);
		FPSCamera* cam = reinterpret_cast<FPSCamera*>(m_lightCamera.get());
		cam->lookAt(-m_pointLight->getPosition(), Camera3D::LocalUp);
	}
	*/
	void RenderSys::setClearMask(GLbitfield mask)
	{
		m_renderState.m_clearMask = mask;
	}

	void RenderSys::setClearColor(glm::vec4 clearColor)
	{
		m_renderState.m_clearColor = clearColor;
	}

	void RenderSys::setCullFace(bool enable, GLenum face)
	{
		m_renderState.m_cullFace = enable;
		m_renderState.m_cullFaceMode = face;
	}

	void RenderSys::setDepthTest(bool enable, GLenum func)
	{
		m_renderState.m_depthTest = enable;
		m_renderState.m_depthFunc = func;
	}

	void RenderSys::setSunLight(glm::vec3 dir, glm::vec3 amb,
		glm::vec3 diff, glm::vec3 spec)
	{
		DirectionalLight* light = new DirectionalLight();
		light->setDirection(dir);
		light->setLightColor(amb, diff, spec);
		m_sunLight = std::shared_ptr<DirectionalLight>(light);
	}

	void RenderSys::setPolygonMode(GLenum mode)
	{
		m_renderState.m_polygonMode = mode;
	}

	void RenderSys::render()
	{
		if (m_renderList == nullptr)
			return;

	
		// polygon mode.
		glPolygonMode(GL_FRONT_AND_BACK, m_renderState.m_polygonMode);

		// cullface setting.
		if (m_renderState.m_cullFace)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
		glCullFace(m_renderState.m_cullFaceMode);

		// depth testing setting.
		if (m_renderState.m_depthTest)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
		glDepthFunc(m_renderState.m_depthFunc);

		// clearing color.
		glClearColor(m_renderState.m_clearColor.x, m_renderState.m_clearColor.y,
			m_renderState.m_clearColor.z, m_renderState.m_clearColor.w);
		glClear(m_renderState.m_clearMask);

		// render the drawable list.
		m_renderList->render(m_camera, m_sunLight, m_lightCamera);

		// render the skydome.
		if (m_skyDome != nullptr)
		{
			glDepthFunc(GL_LEQUAL);
			glCullFace(GL_FRONT);
			m_skyDome->render(m_camera, m_sunLight, m_lightCamera);
		}
	}


}