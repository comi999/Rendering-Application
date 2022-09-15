#include "Camera.hpp"

#include <glm/ext.hpp>

#include "Application.hpp"
#include "Transform.hpp"

Camera::Camera()
	: m_Projection( glm::identity< glm::mat4 >() )
	, m_Dirty( true )
	, m_FOV( glm::radians( 90.0f ) )
	, m_Aspect( 1.0f )
	, m_NearZ( 0.1f )
	, m_FarZ( 1000.0f )
{ }

const glm::mat4& Camera::GetProjection() const
{
	if ( m_Dirty )
	{
		const_cast< Camera* >( this )->m_Projection = glm::perspective( m_FOV, m_Aspect, m_NearZ, m_FarZ );
		const_cast< Camera* >( this )->m_Dirty = false;
	}

	return m_Projection;
}

glm::mat4 Camera::GetView() const
{
	Transform* CameraTransform = GetApplication()->GetComponent< Transform >( GetObject() );
	return glm::lookAt( CameraTransform->GetPosition(), CameraTransform->GetPosition() + glm::vec3( 0, 0, 1 ), glm::vec3( 0, 1, 0 ) ); // glm::inverse( GetApplication()->GetComponent< Transform >( GetObject() )->GetTransformation() );
}

glm::mat4 Camera::GetProjectionView() const
{
	return GetProjection() * GetView();
}