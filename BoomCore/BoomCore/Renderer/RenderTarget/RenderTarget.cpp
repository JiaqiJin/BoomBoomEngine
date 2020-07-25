#include "RenderTarget.h"

#include "../Manager/MeshMgr.h"
#include "../Manager/TextureMgr.h"
#include "../Manager/ShaderMgr.h"

namespace Kawaii
{

	void RenderTarget::renderImp()
	{
		// render each mesh.
		MeshMgr::ptr meshMgr = MeshMgr::getSingleton();
		TextureMgr::ptr textureMgr = TextureMgr::getSingleton();
		for (int x = 0; x < m_meshIndex.size(); ++x)
		{
			if (x < m_texIndex.size())
				textureMgr->bindTexture(m_texIndex[x], 0);
			else
				textureMgr->unBindTexture(m_texIndex[x]);
			meshMgr->drawMesh(m_meshIndex[x], m_instance, m_instanceNum);
		}
	}
	/*
	void SimpleRender::testrender(Camera* camera, Shader::ptr shader)
	{
		shader->bind();
		shader->setInt("skybox", 0);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)800 / (float)600, 0.1f, 100.0f);

		shader->setMat4("model", model);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
		shader->setVec3("cameraPos", camera->Position);

		this->renderImp();
		ShaderMgr::getSingleton()->unBindShader();
	}
	*/
	void SimpleRender::render(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera, Shader::ptr shader)
	{
		if (shader == nullptr)
			shader = ShaderMgr::getSingleton()->getShader(m_shaderIndex);
		shader->bind();
		if (sunLight)
			sunLight->setLightUniform(shader, camera);
		shader->setInt("image", 0);
		// depth map.
		Texture::ptr depthMap = TextureMgr::getSingleton()->getTexture("shadowDepth");
		if (depthMap != nullptr)
		{
			shader->setInt("depthMap", 1);
			depthMap->bind(1);
		}
		// light space matrix.
		if (lightCamera != nullptr)
			shader->setMat4("lightSpaceMatrix",
				lightCamera->getProjectMatrix() * lightCamera->getViewMatrix());
		else
			shader->setMat4("lightSpaceMatrix", glm::mat4(1.0f));
		// object matrix.
		shader->setBool("instance", false);
		shader->setBool("receiveShadow", m_receiveShadow);
		shader->setMat4("modelMatrix", m_transformation.getWorldMatrix());
		shader->setMat4("viewMatrix", camera->getViewMatrix());
		shader->setMat4("projectMatrix", camera->getProjectMatrix());
		this->renderImp();
		ShaderMgr::getSingleton()->unBindShader();
	}
	/*
	void SkyDome::testrender(Camera* camera, Shader::ptr shader)
	{
		shader->bind();
		shader->setInt("skybox", 0);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)800 / (float)600, 0.1f, 100.0f);

		shader->setMat4("model", model);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
		shader->setVec3("cameraPos", camera->Position);

		this->renderImp();
		ShaderMgr::getSingleton()->unBindShader();
	}
	*/
	void SkyDome::render(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera, Shader::ptr shader)
	{
		if (shader == nullptr)
			shader = ShaderMgr::getSingleton()->getShader(m_shaderIndex);
		shader->bind();

		shader->setInt("image", 0);
		shader->setBool("instance", false);
		shader->setBool("receiveShadow", m_receiveShadow);
		shader->setMat4("modelMatrix", m_transformation.getWorldMatrix());
		shader->setMat4("viewMatrix", camera->getViewMatrix());
		shader->setMat4("projectMatrix", camera->getProjectMatrix());
		this->renderImp();
		ShaderMgr::getSingleton()->unBindShader();
	}

}