#pragma once
#include <vector>
#include <string>
#include <map>
#include <glm/glm.hpp>

#include "Resource.hpp"

typedef glm::mat4 Bone;

class Skeleton : public Resource
{
public:

	Skeleton() = default;
	Skeleton( const std::string& a_Path );
	size_t GetBoneCount() const { return m_Bones.size(); }
	const Bone* GetBones() const { return m_Bones.data(); }
	const Bone& operator[]( uint32_t a_Index ) const { return m_Bones[ a_Index ]; }
	const Bone& operator[]( const std::string& a_Name ) const { return m_Bones[ m_Names.find( a_Name )->second ]; }

private:

	std::map< std::string, uint32_t > m_Names;
	std::vector< Bone > m_Bones;
};