#include "FrameBuffer.h"
#include "Texture.h"
#include <iostream>

namespace Kawaii
{
	FrameBuffer::FrameBuffer(int width, int height,const std::vector<std::string>& colorName)
		:m_width(width), m_height(height)
	{
		glGenFramebuffers(1, &m_id);

		//color attachment
		m_colorTexIndex.resize(colorName.size());
		for (int x = 0; x < colorName.size(); x++)
			setupColorFramebuffer(colorName[x], x);

		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
		if (colorName.size() > 0)
		{
			glDrawBuffers(colorName.size(), ColorAttachments);
		}
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::bind()
	{
		glViewport(0, 0, m_width, m_height);
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	}

	void FrameBuffer::unBind(int width, int height)
	{
		if (width == -1)width = m_width;
		if (height == -1)height = m_height;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
	}

	void FrameBuffer::clearFramebuffer()
	{
		glDeleteFramebuffers(1, &m_id);
	}


	void FrameBuffer::setupColorFramebuffer(const std::string& name, unsigned int attachIdx)
	{
		Kawaii::Texture2D::ptr texture;
		
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachIdx, GL_TEXTURE_2D, texture->getTextureId(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}