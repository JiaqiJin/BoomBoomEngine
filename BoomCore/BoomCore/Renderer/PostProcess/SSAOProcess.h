#pragma once

#include "../Light.h"
#include "../Camera/Camera3D.h"
#include "../Manager/FrameBuffer.h"

namespace Kawaii
{
	class SSAOProcess
	{
	private:
		std::vector<glm::vec3> m_samples;
		unsigned int m_screenQuadIndex;

		FrameBuffer::ptr m_ssaoFramebuffer;

	public:


	private:


	};
}