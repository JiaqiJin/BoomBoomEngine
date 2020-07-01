#pragma once

#include <vector>
#include <GL/glew.h>
#include <memory>
#include <string>

namespace Kawaii
{
	const GLenum ColorAttachments[] =
	{
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4,
		GL_COLOR_ATTACHMENT5,
		GL_COLOR_ATTACHMENT6,
		GL_COLOR_ATTACHMENT7
	};

	class FrameBuffer
	{
	public:
		enum BufferType
		{
			DepthBuffer,
			ColorBuffer,
			StencilBuffer
		};

	private:
		unsigned int m_id;
		int m_width, m_height;
		BufferType m_type;
		unsigned int m_depthIndex;
		std::vector<unsigned int> m_colorTexIndex;

	public:
		typedef std::shared_ptr<FrameBuffer> ptr;

		FrameBuffer(int width, int height, const std::vector<std::string>& colorName);
		FrameBuffer(int width, int height, BufferType type);
		virtual ~FrameBuffer() { clearFramebuffer(); }
		
		void bind();
		void unBind(int width = -1, int height = -1);

		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }
		static GLenum getGLtype(BufferType type);
		static GLenum getGLAttach(BufferType type);
		unsigned int getFramebufferId()const { return m_id; }


	private:
		void setupColorFramebuffer(const std::string& name, unsigned int attachIdx);
		void clearFramebuffer();
	};


}

