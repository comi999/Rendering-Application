#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Component.hpp"

class Transform : public Component
{
public:

	Transform();
	inline const glm::vec3& GetPosition() const { return m_Position; }
	inline const glm::quat& GetRotation() const { return m_Rotation; }
	inline const glm::vec3& GetScale() const { return m_Scale; }
	inline void SetPosition( const glm::vec3& a_Position ) { m_Position = a_Position; m_IsDirty = true; }
	inline void SetRotation( const glm::quat& a_Rotation ) { m_Rotation = a_Rotation; m_IsDirty = true; }
	inline void SetScale( const glm::vec3& a_Scale ) { m_Scale = a_Scale; m_IsDirty = true; }
	const glm::mat4& GetTransformation() const;

private:

	bool      m_IsDirty;
	glm::mat4 m_Transform;
	glm::vec3 m_Position;
	glm::quat m_Rotation;
	glm::vec3 m_Scale;
};