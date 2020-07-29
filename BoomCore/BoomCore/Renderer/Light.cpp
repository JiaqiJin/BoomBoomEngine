#include "Light.h"

#include <sstream>

namespace Kawaii
{
	void Light::setLightColor(glm::vec3 radiance)
	{
		m_radiance = radiance;
	}

	void Light::setLightColor(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
	{
		m_ambient = amb;
		m_diffuse = diff;
		m_specular = spec;
	}

	void DirectionalLight::setDirection(glm::vec3 dir)
	{
		m_direction = glm::normalize(dir);
	}

	void DirectionalLight::setLightUniform(Shader::ptr shader, Camera3D::ptr camera)
	{
		shader->setVec3("cameraPos", camera->getPosition());
		shader->setVec3("dirLight.direction", m_direction);
		shader->setVec3("dirLight.ambient", m_ambient);
		shader->setVec3("dirLight.diffuse", m_diffuse);
		shader->setVec3("dirLight.specular", m_specular);
		shader->setVec3("dirLight.radiance", m_radiance);
	}

	glm::vec3 DirectionalLight::getDirection() const
	{
		return m_direction;
	}

	PointLight::PointLight()
	{
		m_atteunationCoff = 0.5f;
		m_radiance = glm::vec3(1.0f);
		m_radius = sqrt(256.0f * 1.0f / (1.0f * m_atteunationCoff));
	}

	void PointLight::setPosition(glm::vec3 pos, unsigned int order)
	{
		m_order = order;
		m_position = pos;
	}

	void PointLight::setAttenuationCoff(float coff)
	{
		m_atteunationCoff = coff;
		// calculate point light's volume.
		GLfloat lightMax = std::fmaxf(std::fmaxf(m_radiance.r, m_radiance.g), m_radiance.b);
		m_radius = sqrt(256.0f * lightMax / (1.0f * m_atteunationCoff));
	}


	void PointLight::setLightColor(glm::vec3 radiance)
	{
		Light::setLightColor(radiance);
		// calculate point light's volume.
		GLfloat lightMax = std::fmaxf(std::fmaxf(m_radiance.r, m_radiance.g), m_radiance.b);
		m_radius = sqrt(256.0f * lightMax / (1.0f * m_atteunationCoff));
	}

	void PointLight::setLightUniform(Shader::ptr shader, Camera3D::ptr camera)
	{
		/*shader->setVec3("cameraPos", camera->getPosition());
		shader->setVec3("pointLight.postion", m_position);
		shader->setVec3("pointLight.ambient", m_ambient);
		shader->setVec3("pointLight.diffuse", m_diffuse);
		shader->setVec3("pointLight.specular", m_specular);
		shader->setFloat("pointLight.radius", m_radius);
		shader->setFloat("lightAttenuationCoff", m_atteunationCoff);
		shader->setFloat("pointLight.constant", 1.0f);
		shader->setFloat("pointLight.linear", 0.09f);
		shader->setFloat("pointLight.quadratic", 0.032f);*/
		std::stringstream ss;
		ss << m_order;
		shader->setVec3("cameraPos", camera->getPosition());
		std::string str = "pointLight[" + ss.str() + "].";
		shader->setVec3(str + "position", m_position);
		shader->setVec3(str + "radiance", m_radiance);
		shader->setFloat(str + "radius", m_radius);
		shader->setFloat("lightAttenuationCoff", m_atteunationCoff);
	}

	glm::vec3 PointLight::getPosition() const
	{
		return m_position;
	}

}