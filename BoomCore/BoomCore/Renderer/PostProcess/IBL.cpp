#include "IBL.h"

#include "../Manager/Geometry.h"
#include "../Manager/Mesh.h"
#include "../Manager/TextureMgr.h"
#include "../Manager/ShaderMgr.h"
#include "../Manager/FrameBuffer.h"

#include <sstream>

namespace Kawaii
{
	void  IBLAux::convertToCubemap(int width, int height, unsigned int hdrTexIndex,
		unsigned int cuebmapTexIndex)
	{
		// manager.
		TextureMgr::ptr texMgr = TextureMgr::getSingleton();
		ShaderMgr::ptr shaderMgr = ShaderMgr::getSingleton();
		//load shader
		unsigned int shaderIndex = shaderMgr->loadShader("convertToCubemap",
			"Shader/convertToCubemap.vs", "Shader/convertToCubemap.fs");
		// load cube mesh.
		Mesh::ptr cubeMesh = std::shared_ptr<Mesh>(new Cube(1.0f, 1.0f, 1.0f));
		//load frane buffer
		FrameBuffer::ptr framebuffer = std::shared_ptr<FrameBuffer>(new FrameBuffer(width, height,
			"convertDepth", {}, true));
		// projection matrix and view matrix.
		glm::mat4 captureProjectMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 captureViewMatrix[] =
		{
			glm::lookAt(glm::vec3(0.0f), glm::vec3(+1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,+1.0f, 0.0f), glm::vec3(0.0f,  0.0f,+1.0f)),
			glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,-1.0f, 0.0f), glm::vec3(0.0f,  0.0f,-1.0f)),
			glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f,+1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f,-1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		};

		// convert.
		framebuffer->bind();
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GLuint cubemapId = texMgr->getTexture(cuebmapTexIndex)->getTextureId();
		Shader::ptr shader = shaderMgr->getShader(shaderIndex);
		shader->bind();
		shader->setInt("hdrMap", 0);
		shader->setMat4("projectMatrix", captureProjectMatrix);
		texMgr->bindTexture(hdrTexIndex, 0);
		for (unsigned int i = 0; i < 6; i++)
		{
			shader->setMat4("viewMatrix", captureViewMatrix[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemapId, 0);
			cubeMesh->draw(false, 0);
		}
		shader->unBind();
		texMgr->unBindTexture(hdrTexIndex);
		framebuffer->unBind();
	}

	void IBLAux::convoluteDiffuseIntegral(int width, int height, unsigned int cubemapTexIndex,
		unsigned int irradianceTexIndex)
	{
		// manager.
		TextureMgr::ptr texMgr = TextureMgr::getSingleton();
		ShaderMgr::ptr shaderMgr = ShaderMgr::getSingleton();
		// load shader.
		unsigned int shaderIndex = shaderMgr->loadShader("diffuseIntegral",
			"Shader/diffuseIntegral.vs", "Shader/diffuseIntegral.fs");
		// load cube mesh.
		Mesh::ptr cubeMesh = std::shared_ptr<Mesh>(new Cube(1.0f, 1.0f, 1.0f));
		// load framebuffer.
		FrameBuffer::ptr framebuffer = std::shared_ptr<FrameBuffer>(
			new FrameBuffer(width, height, "irradianceDepth", {}, true));
		// projection matrix and view matrix.
		glm::mat4 captureProjectMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 captureViewMatrix[] =
		{
			glm::lookAt(glm::vec3(0.0f), glm::vec3(+1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,+1.0f, 0.0f), glm::vec3(0.0f,  0.0f,+1.0f)),
			glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,-1.0f, 0.0f), glm::vec3(0.0f,  0.0f,-1.0f)),
			glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f,+1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f,-1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		};

		// begin to convolute.
		framebuffer->bind();
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GLuint irradianceTexId = texMgr->getTexture(irradianceTexIndex)->getTextureId();
		Shader::ptr shader = shaderMgr->getShader(shaderIndex);
		shader->bind();
		shader->setInt("environmentMap", 0);
		shader->setMat4("projectMatrix", captureProjectMatrix);
		texMgr->bindTexture(cubemapTexIndex, 0);
		for (unsigned int i = 0; i < 6; ++i)
		{
			shader->setMat4("viewMatrix", captureViewMatrix[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceTexId, 0);
			cubeMesh->draw(false, 0);
		}
		shader->unBind();
		texMgr->unBindTexture(cubemapTexIndex);
		framebuffer->unBind();
	}
}