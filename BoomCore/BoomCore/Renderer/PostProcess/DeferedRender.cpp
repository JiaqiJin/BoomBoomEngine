#include "DeferedRender.h"

#include "../Manager/MeshMgr.h"
#include "../Manager/ShaderMgr.h"
#include "../Manager/TextureMgr.h"

namespace Kawaii
{
	DeferedRender::DeferedRender(int width, int height)
	{
		m_screenQuadIndex = MeshMgr::getSingleton()->loadMesh(new ScreenQuad());
		m_deferedShaderIndex = ShaderMgr::getSingleton()->loadShader("pbrDeferedShading",
			"Shaders/PBR/pbrDeferedShading.vs", "Shaders/PBR/pbrDeferedShading.fs");
		m_deferedFramebuffer = std::shared_ptr<FrameBuffer>(new FrameBuffer(
			width, height, "deferedDepth", { "deferedPos", "deferedNormal", "deferedAlbedo",
			"deferedRoughness" }, true));		
	}

	unsigned int DeferedRender::getFrameBufferId()
	{
		return m_deferedFramebuffer->getFramebufferId();
	}

	void DeferedRender::bindDeferedFramebuffer()
	{
		m_deferedFramebuffer->bind();
	}

	void DeferedRender::renderDeferedShading(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera
		, const std::vector<PointLight::ptr>& pointLights)
	{
		Shader::ptr deferedShader = ShaderMgr::getSingleton()->getShader(m_deferedShaderIndex);
		deferedShader->bind();
		deferedShader->setInt("dposition", 0);
		deferedShader->setInt("dnormal", 1);
		deferedShader->setInt("dalbedo", 2);
		deferedShader->setInt("droughness", 3);
		deferedShader->setInt("ddepth", 4);
		deferedShader->setInt("shadowDepth", 5);
		deferedShader->setInt("irradianceMap", 6);
		deferedShader->setInt("prefilteredMap", 7);
		deferedShader->setInt("brdfLutMap", 8);
		deferedShader->setMat4("lightSpaceMatrix", lightCamera->getProjectMatrix() * lightCamera->getViewMatrix());
		sunLight->setLightUniform(deferedShader, camera);
		deferedShader->setInt("pointLightNum", pointLights.size());
		for (unsigned int x = 0; x < pointLights.size(); ++x)
		{
			pointLights[x]->setLightUniform(deferedShader, camera);
		}
		TextureMgr::getSingleton()->bindTexture("deferedPos", 0);
		TextureMgr::getSingleton()->bindTexture("deferedNormal", 1);
		TextureMgr::getSingleton()->bindTexture("deferedAlbedo", 2);
		TextureMgr::getSingleton()->bindTexture("deferedRoughness", 3);
		TextureMgr::getSingleton()->bindTexture("deferedDepth", 4);
		TextureMgr::getSingleton()->bindTexture("shadowDepth", 5);
		TextureMgr::getSingleton()->bindTexture("irradianceMap", 6);
		TextureMgr::getSingleton()->bindTexture("prefilteredMap", 7);
		TextureMgr::getSingleton()->bindTexture("brdfLutMap", 8);
		MeshMgr::getSingleton()->drawMesh(m_screenQuadIndex, false, 0);
		TextureMgr::getSingleton()->unBindTexture("deferedPos");
		TextureMgr::getSingleton()->unBindTexture("deferedNormal");
		TextureMgr::getSingleton()->unBindTexture("deferedAlbedo");
		TextureMgr::getSingleton()->unBindTexture("deferedRoughness");
		TextureMgr::getSingleton()->unBindTexture("deferedDepth");
		TextureMgr::getSingleton()->unBindTexture("shadowDepth");
		TextureMgr::getSingleton()->unBindTexture("irradianceMap");
		TextureMgr::getSingleton()->unBindTexture("prefilteredMap");
		TextureMgr::getSingleton()->unBindTexture("brdfLutMap");
		deferedShader->unBind();
	}

}