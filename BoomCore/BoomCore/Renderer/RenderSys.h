#pragma once

#include "Light.h"
#include "Camera/Camera3D.h"
#include "Manager/ShaderMgr.h"
#include "Manager/TextureMgr.h"
#include "RenderTarget/RenderTarget.h"
#include "Manager/FrameBuffer.h"

namespace Kawaii
{
	class RenderSys
	{
	private:
		DirectionalLight::ptr m_sunLight;

		int m_width, m_height;
		FrameBuffer::ptr m_FrameBuffer;

	public:

	};
}