#include "Scene.h"

#include <time.h>
#include "Renderer/RenderTarget/StaticModelRenderer.h"

using namespace Kawaii;

void SponzaScene::initializeScene(Kawaii::RenderSys::ptr renderSys)
{
	// resource manager.
	MeshMgr::ptr meshMgr = renderSys->getMeshMgr();
	ShaderMgr::ptr shaderMgr = renderSys->getShaderMgr();
	TextureMgr::ptr textureMgr = renderSys->getTextureMgr();

	unsigned int pbrShader = shaderMgr->loadShader("pbrShader",
		"Shaders/PBR/pbrShader.vs", "Shaders/PBR/pbrShader.fs");

	StaticModelRenderer* sponza = new StaticModelRenderer(pbrShader,
		"./res/lamp/lamp.obj");
	
	sponza->getTransformation()->setScale(glm::vec3(1, 1, 1));
	
	renderSys->addRenderTarget(sponza);
	
	renderSys->setSunLight(glm::vec3(0.1f, 1.0f, 0.3f), glm::vec3(1.0f));

	srand(time(nullptr));
	for (unsigned int x = 0; x < 128; ++x)
	{
		glm::vec3 pos;
		glm::vec3 radiance;
		pos.x = -200.0f + (((double)rand()) / RAND_MAX) * 400.0f;
		pos.y = -200.0f + (((double)rand()) / RAND_MAX) * 400.0f;
		pos.y = 2.5f;
		pos.z = -200.0f + (((double)rand()) / RAND_MAX) * 400.0f;
		radiance.x = (((double)rand()) / RAND_MAX) * 80;
		radiance.y = (((double)rand()) / RAND_MAX) * 80;
		radiance.z = (((double)rand()) / RAND_MAX) * 80;

		renderSys->addPointLight(pos, radiance);
	}

}