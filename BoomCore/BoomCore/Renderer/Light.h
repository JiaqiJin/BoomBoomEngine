#pragma once

#include "Manager/Shader.h"
#include "Camera/Camera3D.h"

namespace Kawaii
{
	class Light
	{
	protected:
		glm::vec3 m_radiance;
	public:
		typedef std::shared_ptr<Light> ptr;

		Light() = default;
		~Light() = default;
		glm::vec3 getRadiance() const { return m_radiance; }

		virtual void setLightColor(glm::vec3 radiance);
		virtual void setLightUniform(Shader::ptr shader,Camera3D::ptr camera) = 0;
	};

	class DirectionalLight : public Light
	{
	private:
		glm::vec3 m_direction;

	public:
		typedef std::shared_ptr<DirectionalLight>ptr;

		DirectionalLight() = default;
		virtual ~DirectionalLight() = default;
		
		glm::vec3 getDirection()const;
		void setDirection(glm::vec3 dir);
		virtual void setLightUniform(Shader::ptr shader, Camera3D::ptr camera);
	};

	class PointLight : public Light
	{
	private:
		float m_atteunationCoff;
		glm::vec3 m_position;
		float m_radius;
		unsigned int m_order;

	public:
		typedef std::shared_ptr<PointLight>ptr;

		PointLight();
		virtual ~PointLight() = default;

		glm::vec3 getPosition() const;

		void setAttenuationCoff(float coff);
		virtual void setLightColor(glm::vec3 radiance);
		void setPosition(glm::vec3 pos, unsigned int order);
		virtual void setLightUniform(Shader::ptr shader, Camera3D::ptr camera);
	};

}