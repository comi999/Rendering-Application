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

void RecursiveAdd( aiNode* a_Node, uint32_t a_Parent, const std::map< aiNode*, bool >& a_Dependencies, std::vector< Bone >& o_Bones )
{
	Bone& NewBone = o_Bones.emplace_back();
	NewBone.Name = a_Node->mName.C_Str();
	NewBone.Index = o_Bones.size() - 1u;
	NewBone.Parent = a_Parent;
	Utility::Convert( a_Node->mTransformation, NewBone.Local );

	for ( uint32_t i = 0; i < a_Node->mNumChildren; ++i )
	{
		aiNode* ChildNode = a_Node->mChildren[ i ];

		if ( a_Dependencies.find( ChildNode )->second )
		{
			RecursiveAdd( ChildNode, NewBone.Index, a_Dependencies, o_Bones );
		}
	}
}

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
//		 This node could already be registered as part of a previous node's registration.
//		 If so, continue, don't bother.
//		if ( Dependencies.find( ThisNode->mName.C_Str() ) != Dependencies.end() )
//		{
//			continue;
//		}
//
//		 Create a Node for this aiNode.
//		Node* NewNode = &( Dependencies[ ThisNode->mName.C_Str() ] =
//		{
//			Pair.first,
//			Pair.second,
//			nullptr,
//			{}
//		} );
//
//		 Search up the tree until either you hit the root node of the scene, or you find a node that's
//		 already been registered.
//		while ( true )
//		{
//			 If ThisNode's parent is null, then ThisNode is the RootNode.
//			if ( !ThisNode->mParent )
//			{
//				RootNode = NewNode;
//				break;
//			}
//
//			 Store the NewNode as the previous and set ThisNode to it's parent.
//			PrevNode = NewNode;
//			ThisNode = ThisNode->mParent;
//
//			 First check if the ThisNode already registered.
//			auto Found = Dependencies.find( ThisNode->mName.C_Str() );
//
//			 If it is, then connect PrevNode as a child of the found Node and finish.
//			if ( Found != Dependencies.end() )
//			{
//				Found->second.Children.push_back( PrevNode );
//				PrevNode->Parent = &Found->second;
//				break;
//			}
//
//			 If not, then create a new node for it.
//			NewNode = &( Dependencies[ ThisNode->mName.C_Str() ] =
//			{
//				nullptr,
//				ThisNode,
//				nullptr,
//				{}
//			} );
//
//			 Set up it's relationship.
//			NewNode->Children.push_back( PrevNode );
//			PrevNode->Parent = NewNode;
//		}
//	}
//
//	uint32_t BoneCount = Dependencies.size();
//	PrevNode = nullptr;
//
//    while ( RootNode && !RootNode->Bone )
//	{
//		PrevNode = RootNode;
//		RootNode = RootNode->Children[ 0 ];
//        --BoneCount;
//    }
//
//	RootNode = PrevNode;
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
//	 Start at index 1, because 0 will always be reserved for the root bone.
//	for ( uint32_t i = 1; i < m_Bones.size(); ++i )
//	{
//		Bone& ThisBone = m_Bones[ i ];
//		Bone& ParentBone = m_Bones[ ThisBone.Parent ];
//		ThisBone.Local = glm::mat4( 1.0f );
//		ThisBone.Offset = glm::mat4( 1.0f );// glm::inverse( glm::inverse( ParentBone.Offset )* ThisBone.Local );
//	}
//}

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

	// First register all nodes with associated bones in the dependencies list.
	for ( auto& Pair : ToSearch )
	{
		Dependencies[ Pair.second->mName.C_Str() ] =
		{
			Pair.first,
			Pair.second,
			nullptr,
			{}
		};
	}

	Node* RootNode = nullptr;
	Node* PrevNode = nullptr;

	for ( auto& Pair : ToSearch )
	{
		aiNode* ThisNode = Pair.second;
		Node* Current = &Dependencies[ Pair.second->mName.C_Str() ];

		// Search up the tree until either you hit the root node of the scene, or you find a node that's
		// already been registered.
		while ( true )
		{
			// If ThisNode's parent is null, then ThisNode is the RootNode.
			if ( !ThisNode->mParent )
			{
				RootNode = Current;
				break;
			}

			do
			{
				PrevNode = Current;
				ThisNode = ThisNode->mParent;
				auto Iter = Dependencies.find( ThisNode->mName.C_Str() );
				Current = Iter == Dependencies.end() ? nullptr : &Iter->second;
			} while ( !Current || !Current->Bone );

			//// Store the NewNode as the previous and set ThisNode to it's parent.
			//PrevNode = Current;

			//// Keep searching upwards until you find the next bone node above.
			//while ( Current && !Current->Bone )
			//{
			//	ThisNode = ThisNode->mParent;
			//	auto Iter = Dependencies.find( ThisNode->mName.C_Str() );
			//	Current = Iter == Dependencies.end() ? nullptr : &Iter->second;
			//}

			// First check if the ThisNode already registered.
			auto Found = Dependencies.find( ThisNode->mName.C_Str() );

			// If it is, then connect PrevNode as a child of the found Node and finish.
			if ( Found != Dependencies.end() )
			{
				Found->second.Children.push_back( PrevNode );
				PrevNode->Parent = &Found->second;
				break;
			}

			// If not, then create a new node for it.
			Current = &( Dependencies[ ThisNode->mName.C_Str() ] =
			{
				nullptr,
				ThisNode,
				nullptr,
				{}
			} );

			// Set up it's relationship.
			Current->Children.push_back( PrevNode );
			PrevNode->Parent = Current;
		}
	}

	uint32_t BoneCount = Dependencies.size();
	PrevNode = nullptr;

    while ( RootNode && !RootNode->Bone )
	{
		PrevNode = RootNode;
		RootNode = RootNode->Children[ 0 ];
        --BoneCount;
    }

	RootNode = PrevNode;

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

		for ( Node* Child : ThisNode->Children )
		{
			NodeQueue.push( { Child, ThisBone.Index } );
		}

		m_Names[ ThisBone.Name ] = ThisBone.Index;
		NodeQueue.pop();
	}

	m_Bones[ 0 ].Local = glm::mat4( 1.0f );
	m_Bones[ 0 ].Offset = glm::mat4( 1.0f );

	// Start at index 1, because 0 will always be reserved for the root bone.
	for ( uint32_t i = 1; i < m_Bones.size(); ++i )
	{
		Bone& ThisBone = m_Bones[ i ];
		Bone& ParentBone = m_Bones[ ThisBone.Parent ];
		ThisBone.Local = glm::mat4( 1.0f );
		ThisBone.Offset = glm::mat4( 1.0f );// glm::inverse( glm::inverse( ParentBone.Offset )* ThisBone.Local );
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