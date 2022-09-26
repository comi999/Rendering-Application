#include "Animation.hpp"

#include <assimp/config.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream> // Delete me
Animation::Animation( const std::string& a_Path )
	: Resource( a_Path )
	, m_Duration( 0.0f )
	, m_TicksPerSecond( 0.0f )
{
	Assimp::Importer Importer;
	const aiScene* ThisScene = Importer.ReadFile( a_Path, 0 );
	aiAnimation* ThisAnimation = ThisScene->mAnimations[ 0 ];
	m_TicksPerSecond = ThisAnimation->mTicksPerSecond;
	m_Duration = ThisAnimation->mDuration;
	m_Channels.resize( ThisAnimation->mNumChannels );

	for ( uint32_t i = 0; i < ThisAnimation->mNumChannels; ++i )
	{
		auto& NewChannel = m_Channels[ i ];
		auto* ThisChannel = ThisAnimation->mChannels[ i ];
		NewChannel.Name = ThisAnimation->mChannels[ i ]->mNodeName.C_Str();
		NewChannel.KeyPositions.resize( ThisChannel->mNumPositionKeys );
		NewChannel.KeyRotations.resize( ThisChannel->mNumRotationKeys );
		NewChannel.KeyScales.resize( ThisChannel->mNumScalingKeys );
		std::cout << NewChannel.Name << std::endl;
		for ( uint32_t j = 0; j < ThisChannel->mNumPositionKeys; ++j )
		{
			auto& ThisPositionKey = ThisChannel->mPositionKeys[ j ];
			NewChannel.KeyPositions[ j ].Time = ThisPositionKey.mTime;
			NewChannel.KeyPositions[ j ].Position = { 
				ThisPositionKey.mValue.x, 
				ThisPositionKey.mValue.y, 
				ThisPositionKey.mValue.z };
		}

		for ( uint32_t j = 0; j < ThisChannel->mNumRotationKeys; ++j )
		{
			auto& ThisRotationKey = ThisChannel->mRotationKeys[ j ];
			NewChannel.KeyRotations[ j ].Time = ThisRotationKey.mTime;
			NewChannel.KeyRotations[ j ].Rotation = {
				ThisRotationKey.mValue.w,
				ThisRotationKey.mValue.x,
				ThisRotationKey.mValue.y,
				ThisRotationKey.mValue.z
			};
		}

		for ( uint32_t j = 0; j < ThisChannel->mNumScalingKeys; ++j )
		{
			auto& ThisScalingKey = ThisChannel->mScalingKeys[ j ];
			NewChannel.KeyScales[ j ].Time = ThisScalingKey.mTime;
			NewChannel.KeyScales[ j ].Scale = {
				ThisScalingKey.mValue.x,
				ThisScalingKey.mValue.y,
				ThisScalingKey.mValue.z
			};
		}
	}
}