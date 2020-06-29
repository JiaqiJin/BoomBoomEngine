#include "StaticModelRenderer.h"

#include "../Manager/MeshMgr.h"
#include "../Manager/TextureMgr.h"
#include "../Manager/ShaderMgr.h"

#include <iostream>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace Kawaii
{
	StaticModelRenderer::StaticModelRenderer(unsigned int shaderIndex, const std::string& path)
	{
		m_shaderIndex = shaderIndex;
		loadModel(path);
	}

	StaticModelRenderer::~StaticModelRenderer() 
	{
	}

	void StaticModelRenderer::testrender(Camera* camera, Shader::ptr shader)
	{
		shader->bind();
		shader->setInt("skybox", 0);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)800 / (float)600, 0.1f, 100.0f);

		shader->setMat4("model", model);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
		shader->setVec3("cameraPos", camera->Position);

		this->renderImp();
		ShaderMgr::getSingleton()->unBindShader();
	}

	void StaticModelRenderer::render(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera, Shader::ptr shader)
	{
		if (shader == nullptr)
			shader = ShaderMgr::getSingleton()->getShader(m_shaderIndex);
		shader->bind();

		if (sunLight)
			sunLight->setLightUniform(shader, camera);

		shader->setInt("image", 0);

		if (lightCamera != nullptr)
			shader->setMat4("lightSpaceMatrix", lightCamera->getViewMatrix() * lightCamera->getProjectMatrix());
		else
		{
			shader->setMat4("lightSpaceMatrix", glm::mat4(1.0f));
		}

		shader->setBool("instance", false);
		shader->setMat4("modelMatrix", m_transformation.getWorldMatrix());
		shader->setMat4("viewMatrix", camera->getViewMatrix());
		shader->setMat4("projectMatrix", camera->getProjectMatrix());
		this->renderImp();
		ShaderMgr::getSingleton()->unBindShader();
	}

	void StaticModelRenderer::loadModel(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate
			| aiProcess_FlipUVs | aiProcess_GenNormals);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			return;
		}
		m_directory = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene);
	}

	void StaticModelRenderer::processNode(aiNode* node, const aiScene* scene)
	{
		// process all meshes.
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			unsigned int meshIndex, texIndex = 1000000;
			processMesh(mesh, scene, meshIndex, texIndex);
			this->addMesh(meshIndex);
			if (texIndex != 1000000)
				this->addTexture(texIndex);
		}
		// process children' nodes.
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	void StaticModelRenderer::processMesh(aiMesh* mesh, const aiScene* scene,
		unsigned int& meshIndex, unsigned int& texIndex)
	{
		// process mesh.
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		for (unsigned int x = 0; x < mesh->mNumVertices; ++x)
		{
			Vertex vertex;
			// position.
			vertex.position = glm::vec3(
				mesh->mVertices[x].x, mesh->mVertices[x].y, mesh->mVertices[x].z);
			// normal.
			vertex.normal = glm::vec3(
				mesh->mNormals[x].x, mesh->mNormals[x].y, mesh->mNormals[x].z);
			// texcoord.
			if (mesh->mTextureCoords[0])
				vertex.texcoord = glm::vec2(
					mesh->mTextureCoords[0][x].x, mesh->mTextureCoords[0][x].y);
			else
				vertex.texcoord = glm::vec2(0.0f, 0.0f);
			vertex.color = vertex.normal;
			vertices.push_back(vertex);
			
		}

		for (unsigned int x = 0; x < mesh->mNumFaces; ++x)
		{
			aiFace face = mesh->mFaces[x];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int y = 0; y < face.mNumIndices; ++y)
				indices.push_back(face.mIndices[y]);
		}

		Mesh* target = new Mesh(vertices, indices);
		meshIndex = MeshMgr::getSingleton()->loadMesh(target);

		// process material.
		TextureMgr::ptr textureMgr = TextureMgr::getSingleton();
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			aiString nameStr;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &nameStr);
			std::string name(nameStr.C_Str());
			texIndex = textureMgr->loadTexture2D(name, m_directory + "/" + name);
		}
	}

}