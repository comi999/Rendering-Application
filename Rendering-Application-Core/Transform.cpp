#include "Transform.hpp"
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>

Transform::Transform()
	: m_IsDirty( true )
	, m_Position( 0.0f, 0.0f, 0.0f )
	, m_Rotation( glm::identity< glm::quat >() )
	, m_Scale( 1.0f, 1.0f, 1.0f )
{ }

const glm::mat4& Transform::GetTransformation() const
{
	if ( m_IsDirty )
	{
		const_cast< Transform* >( this )->m_Transform = glm::scale( glm::identity< glm::mat4 >(), m_Scale );
		const_cast< Transform* >( this )->m_Transform *= glm::rotate( glm::angle( m_Rotation ), glm::axis( m_Rotation ) );
		glm::translate( const_cast< Transform* >( this )->m_Transform, m_Position );
		const_cast< Transform* >( this )->m_IsDirty = false;
	}

	return m_Transform;
}