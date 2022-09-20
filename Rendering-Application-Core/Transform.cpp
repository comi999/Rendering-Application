#include "Transform.hpp"
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

Transform::Transform()
	: m_IsDirty( true )
	, m_Local( glm::identity< glm::mat4 >() )
	, m_Global( glm::identity< glm::mat4 >() )
	, m_Position( 0.0f, 0.0f, 0.0f )
	, m_Rotation( glm::identity< glm::quat >() )
	, m_Scale( 1.0f, 1.0f, 1.0f )
	, m_Parent( nullptr )
{ }

void Transform::SetPosition( const glm::vec3& a_Position )
{
	m_Position = a_Position;
	m_IsDirty = true;
}

void Transform::SetRotation( const glm::quat& a_Rotation )
{
	m_Rotation = a_Rotation;
	m_IsDirty = true;
}

void Transform::SetScale( const glm::vec3& a_Scale )
{
	m_Scale = a_Scale;
	m_IsDirty = true;
}

const glm::mat4& Transform::GetLocal() const
{
	if ( m_IsDirty )
	{
		const_cast< Transform* >( this )->m_Local = glm::translate( m_Position ) * glm::mat4_cast( m_Rotation ) * glm::scale( m_Scale );
		const_cast< Transform* >( this )->m_IsDirty = false;
	}

	return m_Local;
}

const glm::mat4& Transform::GetGlobal() const
{
	if ( Transform* Parent = m_Parent )
	{
		if ( m_IsDirty )
		{
			const_cast< Transform* >( this )->m_Global = Parent->GetGlobal() * GetLocal();
		}

		return m_Global;
	}

	return GetLocal();
}

void Transform::SetParent( Transform* a_Transform, bool a_RetainGlobal )
{
	// If already has a parent, need to unset it first.
	if ( Transform* Parent = m_Parent )
	{
		// First, remove self from parent.
		Parent->m_Children.erase( std::find_if( Parent->m_Children.begin(), Parent->m_Children.end(), [&]( auto& Handle ) 
		{ 
			return Handle == GetObject(); 
		} ) );

		// Set parent to null.
		m_Parent = NullObject;

		// Now with no parent and we want to retain global, 
		// then the global/local will be represented by m_Local.
		if ( a_RetainGlobal )
		{
			m_Local = m_IsDirty ? Parent->GetGlobal() * GetLocal() : m_Global;

			// Update the local paramaters since they are the source of truth.
			glm::vec3 Skew;
			glm::vec4 Perspective;
			glm::decompose(
				m_Local, m_Scale, m_Rotation, m_Position, Skew, Perspective );
		}
	}

	// Now set the Parent as the new one.
	m_Parent = a_Transform;

	// If the new parent is not null, set it as the parent.
	if ( a_Transform )
	{
		// Add this transform as a child of the new parent.
		a_Transform->m_Children.emplace_back( this );

		// Update local transform if retain global is required.
		if ( a_RetainGlobal )
		{
			// Local will now no longer represent the global as it does
			// if there is no parent. Offload it to m_Global.
			m_Global = GetLocal();

			// Preserve our global transform by adjusting our local.
			// G = Gp * L  ---->  L = Gp-1 * G
			m_Local = glm::inverse( a_Transform->GetGlobal() ) * m_Global;

			// Update the local paramaters since they are the source of truth.
			glm::vec3 Skew;
			glm::vec4 Perspective;
			glm::decompose(
				m_Local, m_Scale, m_Rotation, m_Position, Skew, Perspective );
		}
		else
		{
			m_Global = a_Transform->GetGlobal() * GetLocal();
		}
	}
}

Transform* Transform::GetChild( uint32_t a_Index ) const
{
	if ( !( a_Index < m_Children.size() ) )
	{
		return nullptr;
	}

	return m_Children[ a_Index ];
}

void Transform::AttachChild( Transform* a_Transform, bool a_RetainGlobal )
{
	a_Transform->SetParent( this, a_RetainGlobal );
}

void Transform::DetachChild( uint32_t a_Index, bool a_RetainGlobal )
{
	if ( !( a_Index < m_Children.size() ) )
	{
		return;
	}

	m_Children[ a_Index ]->SetParent( nullptr, a_RetainGlobal );
}

void Transform::Update( bool a_ForceUpdate )
{
	// If is not dirty and not forced, no update required.
	if ( !a_ForceUpdate && !m_IsDirty ) return;
	
	// Update global transform.
	GetLocal();

	if ( m_Parent )
	{
		m_Global = m_Parent->GetGlobal() * m_Local;
	}

	// Propogate update to all children.
	for ( uint32_t i = 0; i < GetChildCount(); ++i )
	{
		GetChild( i )->Update( true );
	}
}