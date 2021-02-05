#pragma once

#include "RenderTarget/RenderTarget.h"
#include "Manager/FrameBuffer.h"

#include<iostream>

namespace Kawaii
{
	class ShadowSystem
	{
	private:
		bool m_first;
		Camera3D::ptr m_lightCamera;
		FrameBuffer::ptr m_shadowDepthBuffer;
		unsigned int m_shadowDepthShaderIndex;

	public:
		typedef std::shared_ptr<ShadowSystem> ptr;

		ShadowSystem(unsigned int width = 2048, unsigned int height = 2048);
		~ShadowSystem() = default;

		Camera3D::ptr getLightCamera() const { return m_lightCamera; }
		FrameBuffer::ptr getShadowDepthBuffer()const { return m_shadowDepthBuffer; }

		void renderShadow(int width, int height, RenderTarget::ptr drawableList, Light::ptr light);

	private:
		void setShadowCamera(RenderTarget::ptr drawableList, Light::ptr light);
	};
}


