#pragma once

#include "Renderer/RenderSys.h"

class Scene
{
public:
	typedef std::shared_ptr<Scene> ptr;

	Scene() = default;
	virtual ~Scene() = default;

	virtual void initializeScene(Kawaii::RenderSys::ptr renderSys) = 0;
};

class SponzaScene : public Scene
{
public:
	typedef std::shared_ptr<SponzaScene> ptr;

	SponzaScene() = default;
	virtual ~SponzaScene() = default;

	virtual void initializeScene(Kawaii::RenderSys::ptr renderSys);
};