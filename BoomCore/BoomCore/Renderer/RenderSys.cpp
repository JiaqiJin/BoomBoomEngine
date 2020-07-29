#include "RenderSys.h"

#include "Camera/FPSCamera.h"
#include "Camera/TPSCamera.h"

#include "Postprocess/IBL.h"

#include <iostream>
#include <time.h>

using namespace std;

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
		m_pointLightRenderer = nullptr;
		// shadow system.
		setSunLight(glm::vec3(0.1f, 1.0f, 0.3f), glm::vec3(0.6f));
		

		// initialization.
		resize(width, height);
		m_meshMgr = MeshMgr::getSingleton();
		m_shaderMgr = ShaderMgr::getSingleton();
		m_textureMgr = TextureMgr::getSingleton();
		m_renderList = make_shared<RenderTargetList>();

		// load built-in shaders.
		m_shaderMgr->loadShader("shadow", "Shaders/depth.vs", "Shaders/depth.fs");

		// defered shading.
		m_deferedRender = std::shared_ptr<DeferedRender>(new DeferedRender(m_width, m_height));
	}

	void RenderSys::setSkyDomeHdr(const std::string& path)
	{
		if (m_skyDome != nullptr)
			return;
		unsigned int skyboxShader = m_shaderMgr->loadShader("skybox",
			"./glsl/skybox.vert", "./glsl/skybox.frag");
		unsigned int hdrTexIndex = m_textureMgr->loadTexture2DHdr("hdrTex", path);
		unsigned int cubeTexIndex = m_textureMgr->loadTextureCubeHdrRaw("skyboxCubemap", nullptr, 1024, 1024);

		// convert hdrmap to cubemap.
		IBLAux::convertToCubemap(1024, 1024, hdrTexIndex, cubeTexIndex);

		// precompute the irradiance map.
		unsigned int irradianceTexIndex = m_textureMgr->loadTextureCubeHdrRaw("irradianceMap", nullptr, 64, 64);
		IBLAux::convoluteDiffuseIntegral(64, 64, cubeTexIndex, irradianceTexIndex);

		// prefilter the environment map for specular lighting.
		unsigned int prefilteredTexIndex = m_textureMgr->loadTextureCubeHdrRaw("prefilteredMap", nullptr, 256, 256, true);
		IBLAux::convoluteSpecularIntegral(256, 256, cubeTexIndex, prefilteredTexIndex);

		// generate brdf lookup texture.
		unsigned int brdfLutTexIndex = m_textureMgr->loadTexture2DHdrRaw("brdfLutMap", nullptr, 512, 512);
		IBLAux::convoluteSpecularBRDFIntegral(512, 512, brdfLutTexIndex);

		unsigned int mesh = m_meshMgr->loadMesh(new Sphere(1.0f, 10, 10));
		m_skyDome = make_shared<SkyDome>(skyboxShader);
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
			"Shaders/skybox.vert", "Shaders/skybox.frag");
		unsigned int cubeTex = m_textureMgr->loadTextureCube("skybox", path, pFix);
		unsigned int mesh = m_meshMgr->loadMesh(new Sphere(1.0f, 10, 10));
		m_skyDome = make_shared<SkyDome>(skyboxShader);
		m_skyDome->addMesh(mesh);
		PBRMaterial mat;
		mat.m_albedoTexIndex = cubeTex;
		m_skyDome->addPbrTexture(mat);
	}

	Camera3D::ptr RenderSys::createFPSCamera(glm::vec3 pos, glm::vec3 target)
	{
		// create a first person camera.
		FPSCamera* _cam = new FPSCamera(pos);
		_cam->lookAt(glm::normalize(target - pos), Camera3D::LocalUp);
		m_camera = shared_ptr<Camera3D>(_cam);
		return m_camera;
	}

	Camera3D::ptr RenderSys::createTPSCamera(glm::vec3 pos, glm::vec3 target)
	{
		// create a third person camera.
		TPSCamera* _cam = new TPSCamera(target, 0.0f, 30.0f, 3.0f);
		m_camera = shared_ptr<Camera3D>(_cam);
		return m_camera;
	}

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

	void RenderSys::addPointLight(glm::vec3 pos, glm::vec3 radiance)
	{
		if (m_pointLights.size() >= 128)
			return;
		PointLight::ptr pointLight = std::shared_ptr<PointLight>(new PointLight());
		pointLight->setPosition(pos, m_pointLights.size());
		pointLight->setLightColor(radiance);
		m_pointLights.push_back(pointLight);
	}

	void RenderSys::setSunLight(glm::vec3 dir, glm::vec3 radiance)
	{
		DirectionalLight* light = new DirectionalLight();
		light->setDirection(dir);
		light->setLightColor(radiance);
		m_sunLight = shared_ptr<DirectionalLight>(light);
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

	void RenderSys::setPolygonMode(GLenum mode)
	{
		m_renderState.m_polygonMode = mode;
	}

	void RenderSys::render()
	{
		if (m_renderList == nullptr)
			return;

		// point light objects genration.
		if (m_pointLightRenderer == nullptr && m_pointLights.size() > 0)
		{
			m_pointLightRenderer = std::shared_ptr<PointLightRenderer>
				(new PointLightRenderer());
			m_pointLightRenderer->setPointLightRadius(0.7f);
			m_pointLightRenderer->setPointLightPositions(m_pointLights);
		}
		updatePointLightPosition();

		// render to g-buffers.
		{
			m_deferedRender->bindDeferedFramebuffer();

			glClearColor(0.0, 0.0, 1.0, 1.0f);
			glClear(m_renderState.m_clearMask);

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

			// render the drawable list.
			m_renderList->render(m_camera, m_sunLight,m_lightCamera);

			// render the light source.
			if (m_pointLightRenderer != nullptr)
				m_pointLightRenderer->render(m_camera, m_sunLight, nullptr, nullptr);
		}

		{
			glDisable(GL_BLEND);
			glDisable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glDepthMask(GL_TRUE);
			glClearColor(1.0, 1.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			m_deferedRender->renderDeferedShading(m_camera, m_sunLight,
				m_lightCamera, m_pointLights);

			// render the skydome.
			if (m_skyDome != nullptr)
			{
				glEnable(GL_DEPTH_TEST);
				glCullFace(GL_FRONT);
				m_skyDome->render(m_camera, m_sunLight, nullptr);
			}
		}
	}

	// just move light source for demo.
	void RenderSys::updatePointLightPosition()
	{
		// generate random velocity.
		if (m_rndVelForPointLights.empty())
		{
			m_rndVelForPointLights.resize(m_pointLights.size());
			srand(time(nullptr));
			for (unsigned int x = 0; x < m_pointLights.size(); ++x)
			{
				glm::vec3 vel;
				vel.x = (((double)rand()) / RAND_MAX) * 2.0f - 1.0f;
				//vel.y = (((double)rand()) / RAND_MAX) * 2.0f - 1.0f;
				vel.y = 0.0f;
				vel.z = (((double)rand()) / RAND_MAX) * 2.0f - 1.0f;
				m_rndVelForPointLights[x] = vel;
			}
		}

		const float speed = 0.2f;
		for (unsigned int x = 0; x < m_pointLights.size(); ++x)
		{
			glm::vec3 pos = m_pointLights[x]->getPosition();
			pos += m_rndVelForPointLights[x] * speed;
			bool update = false;
			if (pos.x > 200.0f)
			{
				pos.x = 200.0f;
				update = true;
			}
			if (pos.x < -200.0f)
			{
				pos.x = -200.0f;
				update = true;
			}
			if (pos.z > 200.0f)
			{
				pos.z = 200.0f;
				update = true;
			}
			if (pos.z < -200.0f)
			{
				pos.z = -200.0f;
				update = true;
			}
			if (update)
				m_rndVelForPointLights[x] *= (-1);
			m_pointLights[x]->setPosition(pos, x);
		}
		m_pointLightRenderer->setPointLightPositions(m_pointLights);
	}

}