#include <map>
#include <list>
#include <string>
#include <queue>
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

	std::list< std::pair< aiBone*, aiNode* > > ToSearch;

	for ( uint32_t i = 0; i < ThisMesh->mNumBones; ++i )
	{
		aiBone* ThisBone = ThisMesh->mBones[ i ];
		aiNode* ThisNode = ThisScene->mRootNode->FindNode( ThisBone->mName );
		ToSearch.emplace_back( ThisBone, ThisNode );
	}

    struct Node
	{
		aiBone* Bone;
		aiNode* This;
		Node*   Parent;
		std::vector< Node* > Children;
	};

	std::map< std::string, Node > Dependencies;
	Node* RootNode = nullptr;
    uint32_t BoneCount = 0;

	for ( auto& Pair : ToSearch )
	{
		aiNode* ThisNode = Pair.second;

		Node* PrevNode = &( Dependencies[ ThisNode->mName.C_Str() ] =
		{
			Pair.first,
			Pair.second,
			nullptr,
			{}
		} );

        ++BoneCount;

		do
		{
            ThisNode = ThisNode->mParent;
			auto Iter = Dependencies.find( ThisNode->mName.C_Str() );

			if ( Iter != Dependencies.end() )
			{
				Iter->second.Children.push_back( PrevNode );
				PrevNode->Parent = &Iter->second;
				break;
			}

			Node& NewNode = Dependencies[ ThisNode->mName.C_Str() ] =
			{
				nullptr,
				ThisNode,
				nullptr,
				{}
			};

            ++BoneCount;
			NewNode.Children.push_back( PrevNode );
			PrevNode->Parent = &NewNode;
            PrevNode = &NewNode;
			ThisNode = ThisNode->mParent;

            if ( !ThisNode )
            {
                RootNode = &NewNode;
            }

        } while ( ThisNode );
	}

    while ( RootNode && !RootNode->Bone )
    {
        RootNode = RootNode->Children[ 0 ];
        --BoneCount;
    }

	m_Bones.reserve( BoneCount );
	std::queue< std::pair< Node*, int32_t > > NodeQueue;
	NodeQueue.push( { RootNode, -1 } );

	while ( !NodeQueue.empty() )
	{
		Bone& ThisBone = m_Bones.emplace_back();
		Node* ThisNode = NodeQueue.front().first;

		ThisBone.Name = ThisNode->This->mName.C_Str();
		ThisBone.Index = m_Bones.size() - 1u;
		ThisBone.Parent = NodeQueue.front().second;
		
		Utility::Convert( ThisNode->This->mTransformation, ThisBone.Local );
		Utility::Convert( ThisNode->Bone ? ThisNode->Bone->mOffsetMatrix : aiMatrix4x4(), ThisBone.Offset );



		if ( !ThisNode->Bone )
		{
			Node* Current = ThisNode;
			glm::mat4 Local;

			while ( Current != RootNode )
			{
				Utility::Convert( Current->This->mTransformation, Local );
				ThisBone.Offset = Local * ThisBone.Offset;
				Current = Current->Parent;
			}

			ThisBone.Offset = glm::inverse( ThisBone.Offset );
		}
		else
		{
			Utility::Convert( ThisNode->Bone->mOffsetMatrix, ThisBone.Offset );
		}

		for ( Node* Child : ThisNode->Children )
		{
			NodeQueue.push( { Child, ThisBone.Index } );
		}

		m_Names[ ThisBone.Name ] = ThisBone.Index;
		NodeQueue.pop();
	}
}

void Skeleton::AddBone( const std::string& a_Name, int32_t a_Parent, const glm::mat4& a_Transform )
{
	if ( a_Parent != -1 && !( a_Parent < m_Bones.size() ) )
	{
		return;
	}

	Bone& NewBone = m_Bones.emplace_back();
	NewBone.Name = a_Name;
	NewBone.Index = m_Bones.size() - 1u;
	NewBone.Parent = a_Parent;
	NewBone.Local = a_Transform;
	NewBone.Offset = a_Parent == -1 ? glm::mat4( 1.0f ) : glm::inverse( glm::inverse( m_Bones[ a_Parent ].Offset ) * a_Transform );
}

void Skeleton::AddBone( const std::string& a_Name, const std::string& a_Parent, const glm::mat4& a_Transform )
{
	auto Iter = std::find_if( m_Bones.begin(), m_Bones.end(), [&]( const Bone& a_Bone ) 
	{ 
		return a_Bone.Name == a_Parent; 
	} );

	if ( Iter != m_Bones.end() )
	{
		AddBone( a_Name, Iter->Index, a_Transform );
	}
}