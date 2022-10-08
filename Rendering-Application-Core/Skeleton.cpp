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

//void RecursiveAdd( aiNode* a_Node, uint32_t a_Parent, std::vector< Bone >& o_Bones )
//{
//	Bone& NewBone = o_Bones.emplace_back();
//	NewBone.Name = a_Node->mName.C_Str();
//	NewBone.Index = o_Bones.size() - 1u;
//	NewBone.Parent = a_Parent;
//	Utility::Convert( a_Node->mTransformation, NewBone.Local );
//
//	for ( uint32_t i = 0; i < a_Node->mNumChildren; ++i )
//	{
//		RecursiveAdd( a_Node->mChildren[ i ], NewBone.Index, o_Bones );
//	}
//}

//Skeleton::Skeleton( const std::string& a_Path )
//	: Resource( a_Path )
//{
//	Assimp::Importer Importer;
//	const aiScene* ThisScene = Importer.ReadFile( a_Path, 0 );
//	aiMesh* ThisMesh = ThisScene->mMeshes[ 0 ];
//
//	std::list< std::pair< aiBone*, aiNode* > > ToSearch;
//
//	for ( uint32_t i = 0; i < ThisMesh->mNumBones; ++i )
//	{
//		aiBone* ThisBone = ThisMesh->mBones[ i ];
//		aiNode* ThisNode = ThisScene->mRootNode->FindNode( ThisBone->mName );
//		ToSearch.emplace_back( ThisBone, ThisNode );
//	}
//
//    struct Node
//	{
//		aiBone* Bone;
//		aiNode* This;
//		Node*   Parent;
//		std::vector< Node* > Children;
//	};
//
//	std::map< std::string, Node > Dependencies;
//	Node* RootNode = nullptr;
//	Node* PrevNode = nullptr;
//
//	for ( auto& Pair : ToSearch )
//	{
//		aiNode* ThisNode = Pair.second;
//
//		// This node could already be registered as part of a previous node's registration.
//		// If so, continue, don't bother.
//		if ( Dependencies.find( ThisNode->mName.C_Str() ) != Dependencies.end() )
//		{
//			continue;
//		}
//
//		// Create a Node for this aiNode.
//		Node* NewNode = &( Dependencies[ ThisNode->mName.C_Str() ] =
//		{
//			Pair.first,
//			Pair.second,
//			nullptr,
//			{}
//		} );
//
//		// Search up the tree until either you hit the root node of the scene, or you find a node that's
//		// already been registered.
//		while ( true )
//		{
//			// If ThisNode's parent is null, then ThisNode is the RootNode.
//			if ( !ThisNode->mParent )
//			{
//				RootNode = NewNode;
//				break;
//			}
//
//			// Store the NewNode as the previous and set ThisNode to it's parent.
//			PrevNode = NewNode;
//			ThisNode = ThisNode->mParent;
//
//			// First check if the ThisNode already registered.
//			auto Found = Dependencies.find( ThisNode->mName.C_Str() );
//
//			// If it is, then connect PrevNode as a child of the found Node and finish.
//			if ( Found != Dependencies.end() )
//			{
//				Found->second.Children.push_back( PrevNode );
//				PrevNode->Parent = &Found->second;
//				break;
//			}
//
//			// If not, then create a new node for it.
//			NewNode = &( Dependencies[ ThisNode->mName.C_Str() ] =
//			{
//				nullptr,
//				ThisNode,
//				nullptr,
//				{}
//			} );
//
//			// Set up it's relationship.
//			NewNode->Children.push_back( PrevNode );
//			PrevNode->Parent = NewNode;
//		}
//	}
//
//	uint32_t BoneCount = Dependencies.size();
//	PrevNode = nullptr;
//	//Node* PrevNode2 = nullptr;
//
//    while ( RootNode && !RootNode->Bone )
//	{
//		//PrevNode2 = PrevNode;
//		//PrevNode = RootNode;
//		RootNode = RootNode->Children[ 0 ];
//        --BoneCount;
//    }
//
//	//RootNode = PrevNode;
//
//	m_Bones.reserve( BoneCount );
//	std::queue< std::pair< Node*, int32_t > > NodeQueue;
//	NodeQueue.push( { RootNode, -1 } );
//
//	while ( !NodeQueue.empty() )
//	{
//		Bone& ThisBone = m_Bones.emplace_back();
//		Node* ThisNode = NodeQueue.front().first;
//
//		ThisBone.Name = ThisNode->This->mName.C_Str();
//		ThisBone.Index = m_Bones.size() - 1u;
//		ThisBone.Parent = NodeQueue.front().second;
//		
//		Utility::Convert( ThisNode->This->mTransformation, ThisBone.Local );
//
//		for ( Node* Child : ThisNode->Children )
//		{
//			NodeQueue.push( { Child, ThisBone.Index } );
//		}
//
//		m_Names[ ThisBone.Name ] = ThisBone.Index;
//		NodeQueue.pop();
//	}
//
//	m_Bones[ 0 ].Local = glm::mat4( 1.0f );
//	m_Bones[ 0 ].Offset = glm::mat4( 1.0f );
//
//	// Start at index 1, because 0 will always be reserved for the root bone.
//	for ( uint32_t i = 1; i < m_Bones.size(); ++i )
//	{
//		Bone& ThisBone = m_Bones[ i ];
//		Bone& ParentBone = m_Bones[ ThisBone.Parent ];
//
//		ThisBone.Offset = glm::inverse( glm::inverse( ParentBone.Offset ) * ThisBone.Local );
//	}
//}

Skeleton::Skeleton( const std::string& a_Path )
	: Resource( a_Path )
{
	Assimp::Importer Importer;
	const aiScene* ThisScene = Importer.ReadFile( a_Path, 0 );//aiPostProcessSteps::aiProcess_OptimizeGraph | aiPostProcessSteps::aiProcess_OptimizeMeshes );
	aiMesh* ThisMesh = ThisScene->mMeshes[ 0 ];

	struct Node
	{
		aiNode* This;
		aiBone* Bone;
		//glm::mat4 Local;
		Node* Parent;
		std::vector< Node* > Children;
	};

	std::map< aiNode*, Node > ToSearch;

	for ( uint32_t i = 0; i < ThisMesh->mNumBones; ++i )
	{
		aiNode* FoundNode = ThisScene->mRootNode->FindNode( ThisMesh->mBones[ i ]->mName );

		ToSearch[ FoundNode ] = 
		{
			FoundNode,
			ThisMesh->mBones[ i ],
			nullptr,
			{}
		};
	}

	// Add root node to the heirarchy.
	Node* RootNode = &( ToSearch[ ThisScene->mRootNode ] = 
	{
		ThisScene->mRootNode,
		nullptr,
		nullptr,
		{}
	} );

	for ( auto& Pair : ToSearch )
	{
		// I need to search upwards until I hit root node or find another node with bone.
		aiNode* Current = Pair.first->mParent;

		while ( Current )
		{
			// Check if it is another bone in the heirarchy.
			auto Iter = ToSearch.find( Current );

			// If it is a bone node, set up relationship.
			if ( Iter != ToSearch.end() )
			{
				Pair.second.Parent = &Iter->second;
				Iter->second.Children.push_back( &Pair.second );
				break;
			}

			// If not, continue upwards.
			Current = Current->mParent;
		}
	}

	RootNode = RootNode->Children[0];

	std::queue< std::pair< Node*, int32_t > > ToAdd;
	ToAdd.push( { RootNode, 0 } );
	Bone& RootBone = m_Bones.emplace_back();
	RootBone.Name = RootNode->This->mName.C_Str();
	RootBone.Index = 0;
	RootBone.Parent = -1;
	RootBone.Local = glm::mat4( 1.0f );
	RootBone.Offset = glm::mat4( 1.0f );

	while ( !ToAdd.empty() )
	{
		auto& Front = ToAdd.front();
		uint32_t Index = m_Bones.size();

		for ( uint32_t i = 0; i < Front.first->Children.size(); ++i )
		{
			ToAdd.push( { Front.first->Children[ i ], Index } );
		}

		Bone& NewBone = m_Bones.emplace_back();
		NewBone.Name = Front.first->This->mName.C_Str();
		NewBone.Index = Index;
		NewBone.Parent = Front.second;
		
		Utility::Convert( Front.first->This->mTransformation, NewBone.Local );
		//NewBone.Offset = glm::inverse( glm::inverse( m_Bones[ NewBone.Parent ].Offset ) * NewBone.Local );
		Utility::Convert( Front.first->Bone ? Front.first->Bone->mOffsetMatrix : aiMatrix4x4(), NewBone.Offset );
		ToAdd.pop();
		m_Names[ NewBone.Name ] = Index;
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