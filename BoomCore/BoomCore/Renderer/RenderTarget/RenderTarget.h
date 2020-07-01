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
		Transform3D m_transformation;

		unsigned int m_shaderIndex;
		std::vector<unsigned int>m_meshIndex;
		std::vector<unsigned int>m_texIndex;

	public:
		typedef std::shared_ptr<RenderTarget> ptr;

		RenderTarget() = default;
		virtual ~RenderTarget() = default;
		
		virtual void testrender(Camera* camera, Shader::ptr shader = nullptr) = 0;
		virtual void render(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera,Shader::ptr shader = nullptr) = 0;

		void addMesh(unsigned int meshIndex) 
		{ 
			m_meshIndex.push_back(meshIndex);
		}

		void addTexture(unsigned int texIndex)
		{
			m_texIndex.push_back(texIndex);
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

		virtual void testrender(Camera* camera, Shader::ptr shader = nullptr)
		{
			for (auto& it : m_list)
				it->testrender(camera, shader);
		}

		virtual void render(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera, Shader::ptr shader = nullptr)
		{
			for (auto& it : m_list)
			{
				it->render(camera, sunLight, lightCamera, shader);
			}
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

		virtual void testrender(Camera* camera, Shader::ptr shader = nullptr);
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

		virtual void testrender(Camera* camera, Shader::ptr shader = nullptr);
		virtual void render(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera, Shader::ptr shader = nullptr);
	};


}
