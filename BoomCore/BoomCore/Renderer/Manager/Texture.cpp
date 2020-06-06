#include "Texture.h"

#include <vector>
#include <iostream>
#include <GL/glew.h>

#include "stb_image.h"

namespace Kawaii
{
	Texture2D::Texture2D(unsigned char* images, int width, int height, int channel)
		:m_width(width), m_height(height), m_channel(channel)
	{
		// texture unit generation.
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);
		// filter setting.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load the image.
		GLenum format;
		if (m_channel == 1)
			format = GL_RED;
		else if (m_channel == 3)
			format = GL_RGB;
		else if (m_channel == 4)
			format = GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height,
			0, format, GL_UNSIGNED_BYTE, images);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture2D::Texture2D(const std::string& path, glm::vec4 bColor)
	{
		m_borderColor = bColor;
		setupTexture(path, "");
	}

	Texture2D::~Texture2D()
	{
		clearTexture();
	}

	void Texture2D::bind(unsigned int unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	void Texture2D::unBind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture2D::setupTexture(const std::string& path, const std::string& pFix)
	{
		// texture unit generation.
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);
		// filter setting.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &m_borderColor[0]);
		// load the image.
		unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_channel, 0);
		GLenum format;
		if (m_channel == 1)
			format = GL_RED;
		else if (m_channel == 3)
			format = GL_RGB;
		else if (m_channel == 4)
			format = GL_RGBA;
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height,
				0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture at path:" << path << std::endl;
		}
		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture2D::clearTexture()
	{
		glDeleteTextures(1, &m_id);
	}



}