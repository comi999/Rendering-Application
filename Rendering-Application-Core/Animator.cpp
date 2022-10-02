#include <glm/gtx/matrix_decompose.hpp>

#include "Animator.hpp"
#include "LineRenderer.hpp"

Animator::Animator()
	: m_IsPlaying( false )
	, m_IsRepeating( false )
	, m_ElapsedTime( 0.0f )
	, m_PlaybackSpeed( 1.0f )
	, m_Skeleton( nullptr )
	, m_Animation( nullptr )
{ }

void Animator::BuildMatrix( glm::mat4& o_BoneMatrix, uint32_t a_BoneIndex ) const
{
	o_BoneMatrix =
		glm::inverse( GetApplication()->GetComponent< Transform >( GetObject() )->GetGlobal() ) *
		m_BoneTransforms[ a_BoneIndex ]->GetGlobal() *
		( *m_Skeleton )[ a_BoneIndex ]->Offset;
}

void Animator::SetSkeleton( const Skeleton* a_Skeleton )
{
	Transform* ThisTransform = GetApplication()->GetComponent< Transform >( GetObject() );

	// Clean up skeleton.
	if ( m_Skeleton )
	{

	}

	m_Skeleton = a_Skeleton;

	// Set new skeleton.
	if ( m_Skeleton )
	{
		m_BoneTransforms.resize( m_Skeleton->GetBoneCount() );

		for ( uint32_t i = 0; i < m_Skeleton->GetBoneCount(); ++i )
		{
			const Bone* ThisBone = ( *m_Skeleton )[ i ];
			Object NewObject = GetApplication()->Create();
			Transform* NewTransform = GetApplication()->GetComponent< Transform >( NewObject );
			LineRenderer* NewLineRenderer = GetApplication()->AddComponent< LineRenderer >( NewObject );

			glm::vec3 Scale;
			glm::quat Rotation;
			glm::vec3 Position;
			glm::vec3 Skew;
			glm::vec4 Perspective;
			glm::decompose(
				ThisBone->Local,
				Scale, Rotation,
				Position, Skew,
				Perspective );

			NewTransform->SetPosition( Position );
			NewTransform->SetRotation( Rotation );
			NewTransform->SetScale( Scale );

			m_BoneTransforms[ i ] = NewTransform;

			if ( ThisBone->Parent < 0 )
			{
				ThisTransform->AttachChild( NewTransform, false );
				continue;
			}

			m_BoneTransforms[ ThisBone->Parent ]->AttachChild( NewTransform, false );
		}
	}
}

void Animator::SetAnimation( const Animation* a_Animation )
{
	m_AnimationChannelContexts.clear();
	m_Animation = a_Animation;

	if ( !m_Animation )
	{
		return;
	}

	m_AnimationChannelContexts.reserve( m_Animation->GetAnimationChannelCount() );

	for ( uint32_t i = 0; i < m_Animation->GetAnimationChannelCount(); ++i )
	{
		m_AnimationChannelContexts.emplace_back( *( *m_Animation )[ i ] );
	}
}

void Animator::OnTick( float a_DeltaTime )
{
	if ( !m_IsPlaying || !m_Animation || !m_Skeleton )
	{
		return;
	}

	float AttenuatedTime = a_DeltaTime * m_PlaybackSpeed;
	m_ElapsedTime += AttenuatedTime;

	if ( m_ElapsedTime >= m_Animation->GetDuration() )
	{
		if ( !m_IsRepeating )
		{
			m_IsPlaying = false;
		}
		else
		{
			m_ElapsedTime -= m_Animation->GetDuration();

			for ( auto& Context : m_AnimationChannelContexts )
			{
				Context = m_ElapsedTime;
			}

			return;
		}
	}

	// .. Do the animation code.
	for ( uint32_t i = 0; i < m_AnimationChannelContexts.size(); ++i )
	{
		AnimationChannelContext& Context = m_AnimationChannelContexts[ i ];
		Context += AttenuatedTime;
		uint32_t BoneIndex = ( *m_Animation )[ i ]->BoneIndex;
		Transform* BoneTransform = m_BoneTransforms[ BoneIndex ];
		BoneTransform->SetPosition( Context.GetPosition() );
		BoneTransform->SetRotation( Context.GetRotation() );
		BoneTransform->SetScale( Context.GetScale() );
	}
}