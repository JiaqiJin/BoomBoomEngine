#pragma once

#include <memory>
#include <string>
#include <glm/glm.hpp>

namespace Kawaii
{
	class Texture
	{
	protected:
		std::string names;
		unsigned int m_id;

	public:
		typedef std::shared_ptr<Texture> ptr;

		Texture() = default;
		virtual ~Texture() = default;
		
		virtual void bind(unsigned int uint) = 0;
		virtual void unBind() = 0;

		unsigned int getTextureId() { return m_id; }

	private:
		virtual void setupTexture(const std::string& path, const std::string& pFix) = 0;
		virtual void clearTexture() = 0;
	};

	class Texture2D : public Texture
	{
	private:
		glm::vec4 m_borderColor;
		int m_width, m_height, m_channel;

	public:
		typedef std::shared_ptr<Texture2D> ptr;

		Texture2D(unsigned char* images, int width, int height, int channel);
		Texture2D(const std::string& path, glm::vec4 bColor = glm::vec4(1.0f));
		~Texture2D();

		virtual void bind(unsigned int unit);
		virtual void unBind();

	private:
		virtual void setupTexture(const std::string& path, const std::string& pFix);
		virtual void clearTexture();
	};


}
