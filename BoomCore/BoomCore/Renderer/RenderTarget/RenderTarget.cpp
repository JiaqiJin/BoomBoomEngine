#include "RenderTarget.h"

#include "../Manager/MeshMgr.h"
#include "../Manager/TextureMgr.h"
#include "../Manager/ShaderMgr.h"

namespace Kawaii
{

	void RenderTarget::renderImp()
	{
		//render all mesh
		MeshMgr::ptr meshMgr = MeshMgr::getSingleton();
		TextureMgr::ptr textureMgr = TextureMgr::getSingleton();

		for (unsigned int x = 0; x < m_meshIndex.size(); x++)
		{
			if (x < m_texIndex.size())
				textureMgr->bindTexture(m_texIndex[x], x);
			else
				textureMgr->unBindTexture(m_texIndex[x]);

			meshMgr->drawMesh(m_meshIndex[x], m_instance, 0);
		}

	}

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

	void SimpleRender::render(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera, Shader::ptr shader)
	{

	}

}