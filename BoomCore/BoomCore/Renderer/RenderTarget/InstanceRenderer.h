#pragma once

#include "RenderTarget.h"

namespace Kawaii
{
	class InstanceRenderer : public RenderTarget
	{
	private:
		unsigned int m_instanceVBO;

	public:
		InstanceRenderer(unsigned int shaderIndex);
		~InstanceRenderer();

		void setInstanceMatrix(const std::vector<glm::mat4>& instanceMatrice,
			GLenum drawWay = GL_STATIC_DRAW);

		virtual void render(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera, Shader::ptr shader = nullptr);
	
	};

}
