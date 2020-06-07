#pragma once

#include "Mesh.h"

namespace Kawaii
{
	class Cube : public Mesh
	{
	public:
		Cube(float width, float height, float depth);

		virtual ~Cube() = default;
	};
}