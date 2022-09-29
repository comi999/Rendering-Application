#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Resource.hpp"

struct KeyPosition
{
	glm::vec3 Position;
	float     Time;
};

struct KeyRotation
{
	glm::quat Rotation;
	float     Time;
};

struct KeyScale
{
	glm::vec3 Scale;
	float     Time;
};

struct AnimationChannel
{
	std::string                Name;
	uint32_t                   BoneIndex;
	std::vector< KeyPosition > KeyPositions;
	std::vector< KeyRotation > KeyRotations;
	std::vector< KeyScale >    KeyScales;
};

class AnimationChannelContext
{
public:

	AnimationChannelContext( const AnimationChannel& a_AnimationChannel )
		: m_AnimationChannel( &a_AnimationChannel )
		, m_ElapsedTime( 0.0f )
		, m_KeyPositionIndex( 0 )
		, m_KeyRotationIndex( 0 )
		, m_KeyScaleIndex( 0 )
	{ }

	AnimationChannelContext& operator=( float a_ElapsedTime )
	{
		m_ElapsedTime = a_ElapsedTime;

		m_KeyPositionIndex = 0;
		m_KeyRotationIndex = 0;
		m_KeyScaleIndex = 0;

		SeekNext( m_AnimationChannel->KeyPositions, m_KeyPositionIndex, m_ElapsedTime );
		SeekNext( m_AnimationChannel->KeyRotations, m_KeyRotationIndex, m_ElapsedTime );
		SeekNext( m_AnimationChannel->KeyScales,    m_KeyScaleIndex,    m_ElapsedTime );

		return *this;
	}
	
	AnimationChannelContext& operator+=( float a_ElapsedTime )
	{
		m_ElapsedTime += a_ElapsedTime;

		SeekNext( m_AnimationChannel->KeyPositions, m_KeyPositionIndex, m_ElapsedTime );
		SeekNext( m_AnimationChannel->KeyRotations, m_KeyRotationIndex, m_ElapsedTime );
		SeekNext( m_AnimationChannel->KeyScales,    m_KeyScaleIndex,    m_ElapsedTime );

		ENTT_ASSERT( m_KeyPositionIndex != m_AnimationChannel->KeyPositions.size() - 1 );

		return *this;
	}
	
	glm::vec3 GetPosition() const
	{
		const KeyPosition& Left = m_AnimationChannel->KeyPositions[ m_KeyPositionIndex ];
		const KeyPosition& Right = m_AnimationChannel->KeyPositions[ static_cast< size_t >( m_KeyPositionIndex ) + 1 ];
		float t = ( m_ElapsedTime - Left.Time ) / ( Right.Time - Left.Time );
		return Left.Position + t * ( Right.Position - Left.Position );
	}

	glm::quat GetRotation() const
	{
		const KeyRotation& Left = m_AnimationChannel->KeyRotations[ m_KeyRotationIndex ];
		const KeyRotation& Right = m_AnimationChannel->KeyRotations[ static_cast< size_t >( m_KeyRotationIndex ) + 1 ];
		float t = ( m_ElapsedTime - Left.Time ) / ( Right.Time - Left.Time );
		return glm::lerp( Left.Rotation, Right.Rotation, t );
	}

	glm::vec3 GetScale() const
	{
		const KeyScale& Left = m_AnimationChannel->KeyScales[ m_KeyScaleIndex ];
		const KeyScale& Right = m_AnimationChannel->KeyScales[ static_cast< size_t >( m_KeyScaleIndex ) + 1 ];
		float t = ( m_ElapsedTime - Left.Time ) / ( Right.Time - Left.Time );
		return Left.Scale + t * ( Right.Scale - Left.Scale );
	}

private:

	template < typename KeyObject >
	static void SeekNext( const std::vector< KeyObject >& a_KeyFrames, uint32_t& o_Index, float a_ElapsedTime )
	{
		for ( ; o_Index < a_KeyFrames.size() - 1; )
		{
			if ( a_KeyFrames[ static_cast< size_t >( o_Index ) + 1 ].Time <= a_ElapsedTime )
			{
				++o_Index;
			}
			else
			{
				break;
			}
		}

		ENTT_ASSERT( o_Index < a_KeyFrames.size() - 1 );
	}

	const AnimationChannel* m_AnimationChannel;
	float m_ElapsedTime;
	uint32_t m_KeyPositionIndex;
	uint32_t m_KeyRotationIndex;
	uint32_t m_KeyScaleIndex;
};

class Animation : public Resource
{
public:

	Animation() = default;
	Animation( const std::string& a_Path );
	float GetDuration() const { return m_Duration; }
	uint32_t GetAnimationChannelCount() const { return m_Channels.size(); }
	const AnimationChannel* operator[]( size_t a_Index ) const { return &m_Channels[ a_Index ]; }

private:

	float                           m_TicksPerSecond;
	float                           m_Duration;
	std::vector< AnimationChannel > m_Channels;
};