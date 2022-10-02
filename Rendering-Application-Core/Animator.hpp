#pragma once
#include "Component.hpp"
#include "Transform.hpp"
#include "Animation.hpp"
#include "Skeleton.hpp"

class Animator : public Component
{
public:

	Animator();
	inline bool IsDebugEnabled() const { return m_IsDebugEnabled; }
	void DebugEnabled( bool a_Enabled );
	void BuildMatrix( glm::mat4& o_BoneMatrix, uint32_t a_BoneIndex ) const;
	void SetSkeleton( const Skeleton* a_Skeleton );
	void SetAnimation( const Animation* a_Animation );
	const Skeleton* GetSkeleton() const { return m_Skeleton; }
	const Animation* GetAnimation() const { return m_Animation; }
	bool IsPlaying() const { return m_IsPlaying; }
	float GetElapsedTime() const { return m_ElapsedTime; }
	void SetElapsedTime( float a_ElapsedTime ) { m_ElapsedTime = a_ElapsedTime; }
	float GetPlaybackSpeed() const { return m_PlaybackSpeed; }
	void SetPlaybackSpeed( float a_PlaybackSpeed ) { m_PlaybackSpeed = a_PlaybackSpeed; }
	void Repeat( bool a_Repeat ) { m_IsRepeating = a_Repeat; }
	bool IsRepeating() const { return m_IsRepeating; }
	void Play() { m_IsPlaying = true; }
	void Stop() { m_IsPlaying = false; }
	void OnTick( float a_DeltaTime );

private:

	bool                                        m_IsDebugEnabled;
	bool                                        m_IsPlaying;
	bool                                        m_IsRepeating;
	float                                       m_ElapsedTime;
	float                                       m_PlaybackSpeed;
	const Skeleton*                             m_Skeleton;
	const Animation*                            m_Animation;
	std::vector< AnimationChannelContext >      m_AnimationChannelContexts;
	std::vector< ComponentHandle< Transform > > m_BoneTransforms;
};