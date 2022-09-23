#include "Skeleton.hpp"

#include <assimp/config.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

Skeleton::Skeleton( const std::string& a_Path )
	: Resource( a_Path )
{
	Assimp::Importer Importer;
	const aiScene* ThisScene = Importer.ReadFile( a_Path, 0 );
	aiMesh* ThisMesh = ThisScene->mMeshes[ 0 ];
	m_Bones.reserve( ThisMesh->mNumBones );

	for ( uint32_t i = 0; i < ThisMesh->mNumBones; ++i )
	{
		Bone& NewBone = m_Bones.emplace_back();
		aiBone* ThisBone = ThisMesh->mBones[ i ];

		for ( uint32_t x = 0; x < 4; ++x )
		for ( uint32_t y = 0; y < 4; ++y )
		{
			NewBone[ x ][ y ] = ThisBone->mOffsetMatrix[ y ][ x ];
		}
	}
}