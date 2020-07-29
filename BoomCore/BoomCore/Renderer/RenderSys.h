#pragma once

#include "Light.h"
#include "Camera/Camera3D.h"
#include "Manager/ShaderMgr.h"
#include "Manager/MeshMgr.h"
#include "Manager/TextureMgr.h"
#include "RenderTarget/RenderTarget.h"
#include "Manager/FrameBuffer.h"
#include "RenderTarget/PointLightRenderer.h"
#include "PostProcess/DeferedRender.h"

namespace Kawaii
{

	struct RenderState
	{
	public:
		GLenum m_depthFunc;
		GLenum m_polygonMode;
		GLenum m_cullFaceMode;
		glm::vec4 m_clearColor;
		GLbitfield m_clearMask;
		bool m_depthTest, m_cullFace;

		RenderState() :
			m_depthFunc(GL_LESS),
			m_polygonMode(GL_FILL),
			m_cullFaceMode(GL_BACK),
			m_clearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f)),
			m_clearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT),
			m_depthTest(true), m_cullFace(true)
		{}
	};

	class RenderSys
	{
	private:
		// skydome.
		SkyDome::ptr m_skyDome;
		DirectionalLight::ptr m_sunLight;
		PointLight::ptr m_pointLight;
		std::vector<PointLight::ptr> m_pointLights;

		// render state.
		RenderState m_renderState;
		int m_width, m_height;

		FrameBuffer::ptr m_shadowDepthBuffer;
		PointLightRenderer::ptr m_pointLightRenderer;
		DeferedRender::ptr m_deferedRender;
		// manager.
		MeshMgr::ptr m_meshMgr;
		Camera3D::ptr m_camera;
		Camera3D::ptr m_lightCamera;
		ShaderMgr::ptr m_shaderMgr;
		TextureMgr::ptr m_textureMgr;
		RenderTargetList::ptr m_renderList;

	public:
		typedef std::shared_ptr<RenderSys> ptr;

		// ctor/dtor
		RenderSys() = default;
		~RenderSys() = default;

		// Initialization.
		void resize(int width, int height);
		void initialize(int width, int height);
		//void createPointLightCamera(glm::vec3 target, float left, float right,
			//float bottom, float top, float near, float far);
		void createSunLightCamera(glm::vec3 target, float left, float right,
			float bottom, float top, float near, float far);
		void setSkyDome(const std::string& path, const std::string& pFix);
		void setSkyDomeHdr(const std::string& path);
		Camera3D::ptr createFPSCamera(glm::vec3 pos, glm::vec3 target);
		Camera3D::ptr createTPSCamera(glm::vec3 pos, glm::vec3 target);

		// Getter.
		Camera3D::ptr getCamera() { return m_camera; }
		MeshMgr::ptr getMeshMgr() { return m_meshMgr; }
		ShaderMgr::ptr getShaderMgr() { return m_shaderMgr; }
		TextureMgr::ptr getTextureMgr() { return m_textureMgr; }

		// Setter.
		void setPolygonMode(GLenum mode);
		void setClearMask(GLbitfield mask);
		void setClearColor(glm::vec4 clearColor);
		void setCullFace(bool enable, GLenum face);
		void setDepthTest(bool enable, GLenum func);
		void addPointLight(glm::vec3 pos, glm::vec3 radiance);
		void addRenderTarget(RenderTarget* target) { m_renderList->addRenderer(target); }
		void setSunLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);
		void setSunLight(glm::vec3 dir, glm::vec3 radiance);
		void render();

	private:
		std::vector<glm::vec3> m_rndVelForPointLights;
		void updatePointLightPosition();
	};
}