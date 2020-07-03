#pragma once

#include "Light.h"
#include "Camera/Camera3D.h"
#include "Manager/ShaderMgr.h"
#include "Manager/MeshMgr.h"
#include "Manager/TextureMgr.h"
#include "RenderTarget/RenderTarget.h"
#include "Manager/FrameBuffer.h"

namespace Kawaii
{
	class RenderSys
	{
	private:
		int m_width, m_height;
		FrameBuffer::ptr m_FrameBuffer;

		DirectionalLight::ptr m_sunLight;
		SkyDome::ptr m_skyDome;

		MeshMgr::ptr m_meshMgr;
		Camera3D::ptr m_camera;
		Camera3D::ptr m_lightCamera;
		ShaderMgr::ptr m_shaderMgr;
		TextureMgr::ptr m_textureMgr;
		RenderTargetList::ptr m_renderList;

	public:
		typedef std::shared_ptr<RenderSys> ptr;
	
		RenderSys() = default;
		~RenderSys() = default;

		//Initization
		void initialize(int width, int height);
		void setSkyDome(const std::string& path, const std::string& pFix);
		Camera3D::ptr createCamera(glm::vec3 pos, glm::vec3 target);

		//getter
		Camera3D::ptr getCamera() { return m_camera; }
		MeshMgr::ptr getMeshMgr() { return m_meshMgr; }
		ShaderMgr::ptr getShaderMgr() { return m_shaderMgr; }
		TextureMgr::ptr getTextureMgr() { return m_textureMgr; }

		//setter
		void addRenderTarget(RenderTarget* target) { m_renderList->addRender(target); }

		void render();

	private:

	};
}