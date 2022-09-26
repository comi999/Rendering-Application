#include <queue>
#include <list>
#include <assimp/config.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Skeleton.hpp"
#include "Utilities.hpp"

Skeleton::Skeleton( const std::string& a_Path )
	: Resource( a_Path )
{
	Assimp::Importer Importer;
	const aiScene* ThisScene = Importer.ReadFile( a_Path, 0 );
	aiMesh* ThisMesh = ThisScene->mMeshes[ 0 ];
	m_Bones.reserve( ThisMesh->mNumBones );

	std::map< std::string, std::pair< aiNode*, aiBone* > > Lookup; // Name to ( Node/Bone )
	std::queue< std::pair< aiNode*, int32_t > > ToSearch; // ( Node/Parent )
	std::list< aiNode* > ToLink;
	std::list< std::string > Names;
	for ( uint32_t i = 0; i < ThisMesh->mNumBones; ++i )
	{
		aiBone* ThisBone = ThisMesh->mBones[ i ];
		aiNode* ThisNode = ThisScene->mRootNode->FindNode( ThisBone->mName );
		Lookup[ ThisNode->mName.C_Str() ] = { ThisNode, ThisBone };
		ToLink.push_back( ThisNode );
		Names.push_back( ThisNode->mName.C_Str() ); // delete me
	}
	return;
	ToLink.sort( []( aiNode* a_A, aiNode* a_B ) { return a_A->FindNode( a_B->mName ); } );
	ToSearch.emplace( ToLink.front(), -1 );
	m_Bones.emplace_back().Parent = -1;

	while ( !ToSearch.empty() )
	{
		auto ThisNode = ToSearch.front(); ToSearch.pop();
		Bone& NewBone = m_Bones.back();
		NewBone.Name = ThisNode.first->mName.C_Str();
		auto Iter = Lookup.find( ThisNode.first->mName.C_Str() );
		Utility::Convert( Iter->second.second->mOffsetMatrix, NewBone.Offset );
		Utility::Convert( ThisNode.first->mTransformation, NewBone.Local );

		for ( uint32_t i = 0; i < ThisNode.first->mNumChildren; ++i )
		{
			m_Bones.emplace_back().Parent = ThisNode.second + 1;
			ToSearch.emplace( ThisNode.first->mChildren[ i ], m_Bones.size() );
		}
	}
}