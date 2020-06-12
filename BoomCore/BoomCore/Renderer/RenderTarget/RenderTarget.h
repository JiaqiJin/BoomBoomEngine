#pragma once

#include "../Manager/Geometry.h"
#include "../Camera/Camera.h"
#include "../Camera/Transform3D.h"
#include "../Manager/Shader.h"

namespace Kawaii
{
	class RenderTarget
	{
	protected:
		unsigned int m_shaderIndex;
		std::vector<unsigned int>m_meshIndex;

	public:
		typedef std::shared_ptr<RenderTarget> ptr;

		RenderTarget() = default;
		virtual ~RenderTarget() = default;
		
		virtual void testrender(Camera* camera, Shader::ptr shader = nullptr) = 0;
		void addMesh(unsigned int meshIndex) { m_meshIndex.push_back(meshIndex); }

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
	};


}
