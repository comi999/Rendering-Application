#include "Mesh.hpp"

#include <assimp/config.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

Mesh::Mesh( const std::string& a_Path )
{
	Assimp::Importer Importer;
	const aiScene* ThisScene = Importer.ReadFile( a_Path, 
		aiPostProcessSteps::aiProcess_GenNormals |
		aiPostProcessSteps::aiProcess_CalcTangentSpace |
		aiPostProcessSteps::aiProcess_Triangulate |
		aiPostProcessSteps::aiProcess_JoinIdenticalVertices |
		aiPostProcessSteps::aiProcess_EmbedTextures |
		aiPostProcessSteps::aiProcess_RemoveRedundantMaterials |
		aiPostProcessSteps::aiProcess_FindInvalidData |
		aiPostProcessSteps::aiProcess_FixInfacingNormals |
		aiPostProcessSteps::aiProcess_LimitBoneWeights |
		aiPostProcessSteps::aiProcess_PopulateArmatureData );

	if ( !ThisScene || !ThisScene->mNumMeshes )
	{
		return;
	}

	aiMesh* ThisMesh = ThisScene->mMeshes[ 0 ];
	
	if ( ThisMesh->HasFaces() )
	{
		m_Indices.reserve( static_cast< size_t >( ThisMesh->mNumFaces ) * 3 );

		for ( uint32_t i = 0; i < ThisMesh->mNumFaces; ++i )
		{
			m_Indices.push_back( ThisMesh->mFaces[ i ].mIndices[ 0 ] );
			m_Indices.push_back( ThisMesh->mFaces[ i ].mIndices[ 1 ] );
			m_Indices.push_back( ThisMesh->mFaces[ i ].mIndices[ 2 ] );
		}
	}

	if ( ThisMesh->HasPositions() )
	{
		m_Positions.reserve( ThisMesh->mNumVertices );

		for ( uint32_t i = 0; i < ThisMesh->mNumVertices; ++i )
		{
			aiVector3D Position = ThisMesh->mVertices[ i ];
			m_Positions.emplace_back( Position.x, Position.y, Position.z, 1.0f );
		}
	}

	if ( ThisMesh->HasNormals() )
	{
		m_Normals.reserve( ThisMesh->mNumVertices );

		for ( uint32_t i = 0; i < ThisMesh->mNumVertices; ++i )
		{
			aiVector3D Normal = ThisMesh->mNormals[ i ];
			m_Normals.emplace_back( Normal.x, Normal.y, Normal.z, 0.0f );
		}
	}

	if ( ThisMesh->HasTangentsAndBitangents() )
	{
		m_Tangents.reserve( ThisMesh->mNumVertices );
		m_Bitangents.reserve( ThisMesh->mNumVertices );

		for ( uint32_t i = 0; i < ThisMesh->mNumVertices; ++i )
		{
			aiVector3D Tangent = ThisMesh->mTangents[ i ];
			aiVector3D Bitangent = ThisMesh->mBitangents[ i ];
			m_Tangents.emplace_back( Tangent.x, Tangent.y, Tangent.z, 0.0f );
			m_Bitangents.emplace_back( Bitangent.x, Bitangent.y, Bitangent.z, 0.0f );
		}
	}
	
	// Only consider channel 0 for this project.
	if ( ThisMesh->HasTextureCoords( 0 ) )
	{
		m_Texels.reserve( ThisMesh->mNumVertices );
		
		for ( uint32_t i = 0; i < ThisMesh->mNumVertices; ++i )
		{
			aiVector3D Texel = ThisMesh->mTextureCoords[ 0 ][ i ];
			m_Texels.emplace_back( Texel.x, Texel.y );
		}
	}

	// Only consider channel 0 for this project.
	if ( ThisMesh->HasVertexColors( 0 ) )
	{
		m_Colours.reserve( ThisMesh->mNumVertices );
		
		for ( uint32_t i = 0; i < ThisMesh->mNumVertices; ++i )
		{
			aiColor4D Colour = ThisMesh->mColors[ 0 ][ i ];
			m_Colours.emplace_back( Colour.r, Colour.g, Colour.b, Colour.a );
		}
	}

	if ( ThisMesh->HasBones() )
	{
		std::vector< uint32_t > BoneInfluenceCounts;
		BoneInfluenceCounts.resize( ThisMesh->mNumVertices );
		m_BoneIndices.resize( ThisMesh->mNumVertices );
		m_BoneWeights.resize( ThisMesh->mNumVertices );

		for ( uint32_t i = 0; i < ThisMesh->mNumBones; ++i )
		{
			auto* Bone = ThisMesh->mBones[ i ];
			
			for ( uint32_t j = 0; j < Bone->mNumWeights; ++j )
			{
				uint32_t VertexIndex = Bone->mWeights[ j ].mVertexId;
				auto& BoneInfluences = BoneInfluenceCounts[ VertexIndex ];
				auto& BoneIndices = m_BoneIndices[ VertexIndex ];
				auto& BoneWeights = m_BoneWeights[ VertexIndex ];

				if ( BoneInfluences < AI_MAX_BONE_WEIGHTS )
				{
					BoneIndices[ BoneInfluences ] = i;
					BoneWeights[ BoneInfluences ] = Bone->mWeights[ j ].mWeight;
					++BoneInfluences;
				}
			}
		}
	}
}