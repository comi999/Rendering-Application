#include <glm/gtx/matrix_decompose.hpp>

#include "Animator.hpp"
#include "LineRenderer.hpp"

Animator::Animator()
	: m_IsDebugEnabled( false )
	, m_IsPlaying( false )
	, m_IsRepeating( false )
	, m_ElapsedTime( 0.0f )
	, m_PlaybackSpeed( 1.0f )
	, m_Skeleton( nullptr )
	, m_Animation( nullptr )
{ }

void Animator::DebugEnabled( bool a_Enabled )
{
	if ( a_Enabled == m_IsDebugEnabled )
	{
		return;
	}

	m_IsDebugEnabled = a_Enabled;
	Application* ThisApplication = GetApplication();

	for ( auto& ChildBone : m_BoneTransforms )
	{
		ThisApplication->GetComponent< LineRenderer >( ChildBone )->SetEnabled( m_IsDebugEnabled );
	}
}

void Animator::BuildMatrix( glm::mat4& o_BoneMatrix, uint32_t a_BoneIndex ) const
{
	o_BoneMatrix =
		m_BoneTransforms[ a_BoneIndex ]->GetGlobal()
		* ( *m_Skeleton )[ a_BoneIndex ]->Offset;
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
			GetApplication()->AddComponent< LineRenderer >( NewObject )->SetEnabled( m_IsDebugEnabled );

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
				//ThisTransform->AttachChild( NewTransform, false );
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
	/*float in = 0;*/

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

		/*in += a_DeltaTime;
		auto q = glm::identity< glm::quat >();
		auto r = glm::rotate( q, in, glm::vec3( 1, 0, 0 ) );
		BoneTransform->SetRotation( r );*/

		if ( m_IsDebugEnabled )
		{
			//const glm::mat4& GlobalTransform = BoneTransform->GetGlobal();
			//glm::vec3 Start;// = BoneTransform->GetParent()->GetGlobal()[ 3 ];
			//glm::vec3 End;// = BoneTransform->GetGlobal()[ 3 ];

			//glm::vec3 Scale;
			//glm::quat Rotation;
			//glm::vec3 Skew;
			//glm::vec4 Perspective;

			//glm::decompose(
			//	BoneTransform->GetParent()->GetGlobal(),
			//	Scale,
			//	Rotation,
			//	Start,
			//	Skew,
			//	Perspective
			//);

			//glm::decompose(
			//	BoneTransform->GetGlobal(),
			//	Scale,
			//	Rotation,
			//	End,
			//	Skew,
			//	Perspective
			//);

			//glm::mat4 Inv = glm::inverse( BoneTransform->GetParent()->GetGlobal() );

			//LineRenderer* ThisLineRenderer = GetApplication()->GetComponent< LineRenderer >( BoneTransform->GetObject() );
			//ThisLineRenderer->Start = Inv * glm::vec4( Start, 1.0f );
			//ThisLineRenderer->End = Inv * glm::vec4( End, 1.0f );
		}
	}
}