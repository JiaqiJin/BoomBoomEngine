#pragma once

#include "RenderTarget.h"

class aiMesh;
class aiNode;
class aiScene;

namespace Kawaii
{
	class StaticModelRenderer : public RenderTarget
	{
	private:
		glm::vec3 m_min, m_max;
		std::string m_directory;

	public:
		typedef std::shared_ptr<StaticModelRenderer> ptr;

		StaticModelRenderer(unsigned int shaderIndex, const std::string& path);
		~StaticModelRenderer();

		virtual void render(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera, Shader::ptr shader = nullptr);
		virtual void renderDepth(Shader::ptr shader, Camera3D::ptr lightCamera);
	private:
		void loadModel(const std::string& path);
		void processNode(aiNode* node, const aiScene* scene);
		//void processMesh(aiMesh* mesh, const aiScene* scene,
			//unsigned int& meshIndex, unsigned int& texIndex);
		void processMesh(aiMesh* mesh, const aiScene* scene,
			unsigned int& meshIndex, PBRMaterial& pbrMat);
	};

}
