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
	// initialization.
	const int width = 1280, height = 780;
	RenderWindow::ptr window = RenderWindow::getSingleton();
	window->initialize("PBR Renderer", width, height);
	RenderSys::ptr renderSys = window->getRenderSystem();


	Camera3D::ptr camera = renderSys->createFPSCamera(glm::vec3(0, 50, 100),glm::vec3(0.0f));
	camera->setPerspectiveProject(45.0f, static_cast<float>(width) / height, 0.1f, 100.0f);
	FPSCamera* fpsCamera = reinterpret_cast<FPSCamera*>(camera.get());

	Scene::ptr scene = shared_ptr<SponzaScene>(new SponzaScene());
	scene->initializeScene(renderSys);

	renderSys->createSunLightCamera(glm::vec3(0.0f), -25.0f, +25.0f,
		-25.0f, +25.0f, 1.0f, 300.0f);

	renderSys->setSkyDomeHdr("res/Hdr/Mans_Outside_2k.hdr");

	while (window->run())
	{
		window->beginFrame();

		renderSys->render();

		window->endFrame();
	}

	window->shutdown();

	return 0;
}