#include "Geometry.h"

#include <glm/glm.hpp>

#define M_PI 3.1415926535
#define M_2PI 6.2831853071

namespace Kawaii
{
	Cube::Cube(float width, float height, float depth)
	{
		float halfW = width * 0.5f;
		float halfH = height * 0.5f;
		float halfD = depth * 0.5f;

		std::vector<Vertex>  vertices =
		{
			Vertex(-halfW, -halfH, -halfD, 0.f, 0.f, -1.f, 0.f, 1.f, 1.0, 0.0, 0.0),
			Vertex(-halfW, +halfH, -halfD, 0.f, 0.f, -1.f, 0.f, 0.f, 0.0, 0.0, 1.0),
			Vertex(+halfW, +halfH, -halfD, 0.f, 0.f, -1.f, 1.f, 0.f, 0.0, 1.0, 0.0),
			Vertex(+halfW, -halfH, -halfD, 0.f, 0.f, -1.f, 1.f, 1.f, 1.0, 0.0, 1.0),

			Vertex(-halfW, -halfH, +halfD, -1.f, 0.f, 0.f, 0.f, 1.f, 1.0, 0.0, 0.0),
			Vertex(-halfW, +halfH, +halfD, -1.f, 0.f, 0.f, 0.f, 0.f, 0.0, 0.0, 1.0),
			Vertex(-halfW, +halfH, -halfD, -1.f, 0.f, 0.f, 1.f, 0.f, 0.0, 1.0, 1.0),
			Vertex(-halfW, -halfH, -halfD, -1.f, 0.f, 0.f, 1.f, 1.f, 0.0, 1.0, 0.0),

			Vertex(+halfW, -halfH, +halfD, 0.f, 0.f, 1.f, 0.f, 1.f, 1.0, 0.0, 0.0),
			Vertex(+halfW, +halfH, +halfD, 0.f, 0.f, 1.f, 0.f, 0.f, 0.0, 1.0, 0.0),
			Vertex(-halfW, +halfH, +halfD, 0.f, 0.f, 1.f, 1.f, 0.f, 0.0, 0.0, 1.0),
			Vertex(-halfW, -halfH, +halfD, 0.f, 0.f, 1.f, 1.f, 1.f, 1.0, 1.0, 0.0),

			Vertex(+halfW, -halfH, -halfD, 1.f, 0.f, 0.f, 0.f, 1.f, 1.0, 0.0, 0.0),
			Vertex(+halfW, +halfH, -halfD, 1.f, 0.f, 0.f, 0.f, 0.f, 1.0, 0.0, 0.0),
			Vertex(+halfW, +halfH, +halfD, 1.f, 0.f, 0.f, 1.f, 0.f, 1.0, 0.0, 0.0),
			Vertex(+halfW, -halfH, +halfD, 1.f, 0.f, 0.f, 1.f, 1.f, 1.0, 0.0, 0.0),

			Vertex(-halfW, +halfH, -halfD, 0.f, 1.f, 0.f, 0.f, 1.f, 1.0, 0.0, 0.0),
			Vertex(-halfW, +halfH, +halfD, 0.f, 1.f, 0.f, 0.f, 0.f, 0.0, 0.0, 1.0),
			Vertex(+halfW, +halfH, +halfD, 0.f, 1.f, 0.f, 1.f, 0.f, 1.0, 0.0, 0.0),
			Vertex(+halfW, +halfH, -halfD, 0.f, 1.f, 0.f, 1.f, 1.f, 0.0, 1.0, 1.0),

			Vertex(-halfW, -halfH, +halfD, 0.f, -1.f, 0.f, 0.f, 1.f, 1.0, 0.0, 0.0),
			Vertex(-halfW, -halfH, -halfD, 0.f, -1.f, 0.f, 0.f, 0.f, 0.0, 1.0, 0.0),
			Vertex(+halfW, -halfH, -halfD, 0.f, -1.f, 0.f, 1.f, 0.f, 0.0, 0.0, 1.0),
			Vertex(+halfW, -halfH, +halfD, 0.f, -1.f, 0.f, 1.f, 1.f, 1.0, 0.0, 1.0)
		};

		std::vector<unsigned int> indices =
		{
			//! front.
			 0, 1, 2,
			 0, 2, 3,

			 //! back.
			 4, 5, 6,
			 4, 6, 7,

			 //! left.
			 8, 9,10,
			 8,10,11,

			 //! right.
			 12,13,14,
			 12,14,15,

			 //! up.
			 16,17,18,
			 16,18,19,

			 //! down.
			 20,21,22,
			 20,22,23
		};

		setupMesh(vertices, indices);
	}
}