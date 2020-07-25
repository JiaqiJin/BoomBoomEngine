#include <iostream>
#include "Renderer/Manager/Shader.h"
#include "Renderer/Manager/Texture.h"
#include "Renderer/Camera/Camera.h"
#include "Renderer/Manager/Geometry.h"
#include "Renderer/Manager/Mesh.h"
#include "Renderer/Manager/TextureMgr.h"
#include "Renderer/Camera/FPSCamera.h"
#include "Renderer/Camera/TPSCamera.h"
#include "Renderer/Camera/Camera3D.h"
#include "Renderer/RenderTarget/RenderTarget.h"
#include "Renderer/Manager/ShaderMgr.h"
#include "Renderer/Manager/MeshMgr.h"
#include "Renderer/RenderWindow.h"
#include "Renderer/RenderTarget/StaticModelRenderer.h"
#include "Renderer/RenderTarget/InstanceRender.h"

using namespace std;
using namespace Kawaii;


int main(int argc, char* argv[])
{
	// initialization.
	const int width = 1000, height = 800;
	RenderWindow::ptr window = RenderWindow::getSingleton();
	window->initialize("Kawakawadesune", width, height);
	RenderSys::ptr renderSys = window->getRenderSystem();

	// resource loading.
	MeshMgr::ptr meshMgr = renderSys->getMeshMgr();
	ShaderMgr::ptr shaderMgr = renderSys->getShaderMgr();
	TextureMgr::ptr textureMgr = renderSys->getTextureMgr();
	// shaders.
	unsigned int simpleColorShader = shaderMgr->loadShader("simple_color",
		"Shaders/simple_color.vs", "Shaders/simple_color.fs");
	//unsigned int simpleTextShader = shaderMgr->loadShader("simple_texture",
		//"Shaders/simple_texture.vert", "Shaders/simple_texture.frag");
	unsigned int blinnPhongShader = shaderMgr->loadShader("blinn_phong",
		"Shaders/blinn_phong.vs", "Shaders/blinn_phong.fs");
	// textures.
	unsigned int tex1 = textureMgr->loadTexture2D("floor", "res/floor.jpg");
	unsigned int cube1 = textureMgr->loadTexture2D("cube", "res/cube.jpg");
	unsigned int blue = textureMgr->loadTexture2D("blue", "res/blue.png");
	renderSys->setSkyDome("res/skybox/", ".png");

	//meshes
	unsigned int planeMesh = meshMgr->loadMesh(new Plane(500.0f, 500.0f));
	unsigned int cubeMesh = meshMgr->loadMesh(new Cube(1.0f, 1.0f, 1.0f));
	unsigned int sphereMesh1 = meshMgr->loadMesh(new Sphere(1.0f, 10, 10));
	unsigned int sphereMesh2 = meshMgr->loadMesh(new Sphere(0.5f, 10, 10));
	//sunlight
	renderSys->setSunLight(glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(0.6f));

	// add render objects.
	RenderTarget *floor = new SimpleRender(blinnPhongShader);
	floor->addMesh(planeMesh);

	StaticModelRenderer* model = new StaticModelRenderer(blinnPhongShader,
		"res/nanosuit/nanosuit.obj");
	Transform3D* trans = model->getTransformation();
	trans->scale(glm::vec3(0.2f));
	trans->translate(glm::vec3(2.5, 0.0, -2.0));

	renderSys->addRenderTarget(floor);
	renderSys->addRenderTarget(model);

	Camera3D::ptr camera = renderSys->createTPSCamera(glm::vec3(0, 100, 200),glm::vec3(0.0f));
	camera->setPerspectiveProject(45.0f, static_cast<float>(width) / height, 0.1f, 100.0f);
	TPSCamera* tpsCamera = reinterpret_cast<TPSCamera*>(camera.get());
	tpsCamera->setPitch(15.0f);
	tpsCamera->setDistance(7.0f);


	renderSys->createSunLightCamera(glm::vec3(0.0f), -25.0f, +25.0f,
		-25.0f, +25.0f, 1.0f, 300.0f);

	while (window->run())
	{
		window->beginFrame();
		renderSys->render();

		window->endFrame();
	}

	window->shutdown();

	return 0;

}
