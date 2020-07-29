#pragma once

#include "../Light.h"
#include "../Camera/Camera3D.h"
#include "../Manager/Geometry.h"
#include "../Manager/FrameBuffer.h"

namespace Kawaii
{
	class DeferedRender
	{
	private:
		unsigned int m_screenQuadIndex;
		unsigned int m_deferedShaderIndex;
		FrameBuffer::ptr m_deferedFramebuffer;

	public:
		typedef std::shared_ptr<DeferedRender> ptr;

		DeferedRender(int width, int height);
		~DeferedRender() = default;

		unsigned int getFrameBufferId();
		void bindDeferedFramebuffer();
		void renderDeferedShading(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera
			, const std::vector<PointLight::ptr>& pointLights);
	};
}