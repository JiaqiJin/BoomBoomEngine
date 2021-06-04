#include <iostream>

#include "Renderer/RenderWindow.h"
#include "Renderer/Camera/TPSCamera.h"
#include "Renderer/Camera/FPSCamera.h"
#include "Renderer/Manager/Geometry.h"
#include "Renderer/RenderTarget/StaticModelRenderer.h"

#include "Scene.h"

using namespace std;
using namespace Kawaii;

int main(int argc, char* argv[])
{
	//initialization.
	const int width = 1000, height = 800;
	RenderWindow::ptr window = RenderWindow::getSingleton();
	window->initialize("Kawaii Desune", width, height);
	RenderSys::ptr renderSys = window->getRenderSystem();

	// resource loading.
	MeshMgr::ptr meshMgr = renderSys->getMeshMgr();
	ShaderMgr::ptr shaderMgr = renderSys->getShaderMgr();
	TextureMgr::ptr textureMgr = renderSys->getTextureMgr();
	// shaders.
	unsigned int simpleColorShader = shaderMgr->loadShader("simple_color",
		"Shaders/simple_color.vs", "Shaders/simple_color.fs");
	unsigned int lightShader = shaderMgr->loadShader("Direclight",
		"Shaders/reflect.vs", "Shaders/reflect.fs");

	// textures.
	unsigned int tex1 = textureMgr->loadTexture2D("floor", "res/floor.jpg");
	unsigned int cube1 = textureMgr->loadTexture2D("cube", "res/cube.jpg");
	unsigned int blue = textureMgr->loadTexture2D("blue", "res/blue.png");

	renderSys->setSkyDome("res/skybox/", ".png");
	// meshes.
	unsigned int planeMesh = meshMgr->loadMesh(new Plane(10.0f, 10.0f));
	unsigned int cubeMesh = meshMgr->loadMesh(new Cube(1.0f, 1.0f, 1.0f));

	// sunlight.
	renderSys->setSunLight(glm::vec3(0.1f, 1.0f, 0.3f), glm::vec3(1.0f));

	// add drawable objects.
	RenderTarget* floor = new SimpleDrawable(lightShader);
	floor->addMesh(planeMesh);
	//floor->addTexture(tex1);

	//StaticModelRenderer* model = new StaticModelRenderer(lightShader,
		//"res/sponza_pbr/sponza_pbr.obj");
	StaticModelRenderer* model = new StaticModelRenderer(lightShader,
		"res/nanosuit/nanosuit.obj");
	Transform3D* trans = model->getTransformation();
	trans->scale(glm::vec3(0.01f));
	trans->translate(glm::vec3(2.5, 0.0, -2.0));

	RenderTarget* box = new SimpleDrawable(lightShader);
	box->addMesh(cubeMesh);
	//box->addTexture(cube1);
	box->getTransformation()->translate(glm::vec3(-3.0f, 0.5f, -2.0f));

	renderSys->addRenderTarget(floor);
	//renderSys->addRenderTarget(model);
	//renderSys->addRenderTarget(box);


	Camera3D::ptr camera = renderSys->createFPSCamera(glm::vec3(0, 5, 10), glm::vec3(0, 0, 0));
	camera->setPerspectiveProject(45.0f, static_cast<float>(width) / height, 0.1f, 100.0f);
	FPSCamera* fpsCamera = reinterpret_cast<FPSCamera*>(camera.get());
	//fpsCamera->setMoveSpeed(100);

	renderSys->createSunLightCamera(glm::vec3(0.0f), -25.0f, +25.0f,
		-25.0f, +25.0f, 1.0f, 300.0f);

	// renderer loop.
	while (window->run())
	{
		window->beginFrame();
		renderSys->render();

		window->endFrame();
	}

	window->shutdown();

	return 0;
}