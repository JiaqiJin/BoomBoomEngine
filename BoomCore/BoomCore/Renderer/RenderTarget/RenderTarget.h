#pragma once

#include <vector>
#include <memory>

#include "../Manager/Geometry.h"
#include "../Light.h"
#include "../Camera/Camera3D.h"
#include "../Camera/Transform3D.h"
#include "AABBBoundingBox.h"

namespace Kawaii
{
	class PBRMaterial
	{
	public:
		unsigned int m_albedoTexIndex = -1;
		unsigned int m_normalTexIndex = -1;
		unsigned int m_roughTexIndex = -1;
		unsigned int m_metallicIndex = -1;
	};

	class RenderTarget
	{
	protected:
		bool m_instance = false;
		bool m_receiveShadow = true;
		bool m_produceShadow = true;
		bool m_visiable = true;
		int m_instanceNum = 0;
		Transform3D m_transformation;
		AABBBoundingBox m_boundingBox;

		unsigned int m_shaderIndex;
		std::vector<PBRMaterial> m_texIndex;
		std::vector<unsigned int> m_meshIndex;

	public:
		typedef std::shared_ptr<RenderTarget> ptr;

		RenderTarget() = default;
		virtual ~RenderTarget() = default;

		virtual AABBBoundingBox& getBoundingBox();

		virtual void render(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera,
			Shader::ptr shader = nullptr) = 0;
		virtual void renderDepth(Shader::ptr shader, Camera3D::ptr lightCamera) = 0;

		void setVisiable(bool target) { m_visiable = target; }
		bool isVisiable()const { return m_visiable; }
		void setProduceShadow(bool target) { m_produceShadow = target; }
		void setReceiveShadow(bool target) { m_receiveShadow = target; }

		void addPbrTexture(PBRMaterial matIndex) { m_texIndex.push_back(matIndex); }
		void addMesh(unsigned int meshIndex) { m_meshIndex.push_back(meshIndex); }
		Transform3D* getTransformation() { return &m_transformation; }

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

		unsigned int addRenderer(RenderTarget* object)
		{
			RenderTarget::ptr renderer(object);
			m_list.push_back(renderer);
			return m_list.size() - 1;
		}

		virtual void render(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera,
			Shader::ptr shader = nullptr)
		{
			for (auto& it : m_list)
				it->render(camera, sunLight, lightCamera, shader);
		}

		virtual void renderDepth(Shader::ptr shader, Camera3D::ptr lightCamera)
		{
			for (auto& it : m_list)
				it->renderDepth(shader, lightCamera);
		}

		virtual AABBBoundingBox& getBoundingBox();

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

		virtual void render(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera,
			Shader::ptr shader = nullptr);
		virtual void renderDepth(Shader::ptr shader, Camera3D::ptr lightCamera) {}
	};

	class SimpleDrawable : public RenderTarget
	{
	public:

		SimpleDrawable(unsigned int shaderIndex)
		{
			m_shaderIndex = shaderIndex;
		}

		~SimpleDrawable() = default;

		virtual void render(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera,
			Shader::ptr shader = nullptr);
		virtual void renderDepth(Shader::ptr shader, Camera3D::ptr lightCamera);
	};


}

//