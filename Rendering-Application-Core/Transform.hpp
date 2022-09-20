#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Component.hpp"
#include "Application.hpp"

class Transform : public Component
{
public:

	Transform();
	inline const glm::vec3& GetPosition() const { return m_Position; }
	inline const glm::quat& GetRotation() const { return m_Rotation; }
	inline const glm::vec3& GetScale() const { return m_Scale; }
	void SetPosition( const glm::vec3& a_Position );
	void SetRotation( const glm::quat& a_Rotation );
	void SetScale( const glm::vec3& a_Scale );
	const glm::mat4& GetLocal() const;
	const glm::mat4& GetGlobal() const;
	inline Transform* GetParent() const { return m_Parent; }
	void SetParent( Transform* a_Parent, bool a_RetainGlobal = true );
	inline uint32_t GetChildCount() const { return m_Children.size(); }
	Transform* GetChild( uint32_t a_Index ) const;
	void AttachChild( Transform* a_Transform, bool a_RetainGlobal = true );
	void DetachChild( uint32_t a_Index, bool a_RetainGlobal = true );
	void Update( bool a_ForceUpdate = false );

private:

	bool                                        m_IsDirty;
	glm::mat4                                   m_Local;
	glm::mat4                                   m_Global;
	glm::vec3                                   m_Position;
	glm::quat                                   m_Rotation;
	glm::vec3                                   m_Scale;
	ComponentHandle< Transform >                m_Parent;
	std::vector< ComponentHandle< Transform > > m_Children;
};