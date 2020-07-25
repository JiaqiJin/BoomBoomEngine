#pragma once

#include "../Camera/Camera.h"

#include "../Manager/Geometry.h"
#include "../Camera/Camera3D.h"
#include "../Camera/Transform3D.h"
#include "../Light.h"

namespace Kawaii
{
	class RenderTarget
	{
	protected:
		bool m_instance = false;
		bool m_receiveShadow = true;
		bool m_visiable = true;
		int m_instanceNum = 0;
		Transform3D m_transformation;

		unsigned int m_shaderIndex;
		std::vector<unsigned int> m_texIndex;
		std::vector<unsigned int> m_meshIndex;

	public:
		typedef std::shared_ptr<RenderTarget> ptr;

		RenderTarget() = default;
		virtual ~RenderTarget() = default;

		virtual void render(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera, Shader::ptr shader = nullptr) = 0;

		void setVisiable(bool target) { m_visiable = target; }
		void setReceiveShadow(bool target) { m_receiveShadow = target; }

		void addTexture(unsigned int texIndex)
		{
			m_texIndex.push_back(texIndex);
		}

		void addMesh(unsigned int meshIndex)
		{
			m_meshIndex.push_back(meshIndex);
		}

		Transform3D* getTransformation()
		{
			return &m_transformation;
		}

	protected:
		void renderImp();
	};

	class RenderTargetList : public RenderTarget
	{
	private:
		std::vector<RenderTarget::ptr> m_list;

	public:
		typedef std::shared_ptr<RenderTargetList> ptr;

		RenderTargetList() = default;
		virtual ~RenderTargetList() = default;

		unsigned int addRender(RenderTarget* object)
		{
			RenderTarget::ptr renderer(object);
			m_list.push_back(renderer);
			return m_list.size() - 1;
		}

		virtual void render(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera, Shader::ptr shader = nullptr)
		{
			for (auto& it : m_list)
				it->render(camera, sunLight, lightCamera, shader);
		}

	};

	class SkyDome : public RenderTarget
	{
	public:
		typedef std::shared_ptr<SkyDome> ptr;

		SkyDome(unsigned int shaderIndex)
		{
			m_shaderIndex = shaderIndex;
		}

		SkyDome() = default;

		virtual void render(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera, Shader::ptr shader = nullptr);
	};

	class SimpleRender : public RenderTarget
	{
	public:

		SimpleRender(unsigned int shaderIndex)
		{
			m_shaderIndex = shaderIndex;
		}

		~SimpleRender() = default;

		virtual void render(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera, Shader::ptr shader = nullptr);
	};

}
