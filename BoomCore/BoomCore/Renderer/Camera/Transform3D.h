#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Kawaii
{
	class Transform3D
	{
	private:
		mutable bool m_dirty;

		glm::vec3 m_scale;
		glm::vec3 m_translation;
		glm::quat m_rotation;
		glm::mat4 m_worldMatrix;
		glm::mat3 m_normalMatrix;

	public:

		Transform3D();
		~Transform3D() = default;

		static const glm::vec3 LocalForward;
		static const glm::vec3 LocalUp; //Up axis
		static const glm::vec3 LocalRight; //Right axis

		// Getter.
		bool isDirty() { return m_dirty; }
		glm::mat4 getWorldMatrix();
		glm::mat3 getNormalMatrix();
		glm::mat4 getInvWorldMatrix();

		glm::vec3 scale() const { return m_scale; }
		glm::quat rotation() const { return m_rotation; }
		glm::vec3 translation() const { return m_translation; }

		//Setter
		void scale(const glm::vec3& ds);
		void translate(const glm::vec3& dt);
		void rotate(const glm::vec3& axis, float angle);
		void setScale(const glm::vec3& s);
		void setRotation(const glm::quat& r);
		void setTranslation(const glm::vec3& t);

		glm::vec3 forward() const;
		glm::vec3 up() const;
		glm::vec3 right() const;

		
	};
}