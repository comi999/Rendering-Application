#pragma once
#include <vector>
#include <string>
#include <map>
#include <glm/glm.hpp>

#include "Resource.hpp"

struct Bone
{
	std::string Name;
	int32_t     Index;
	int32_t     Parent;
	glm::mat4   Local;
	glm::mat4   Offset;
};

class Skeleton : public Resource
{
public:

	Skeleton() = default;
	Skeleton( const std::string& a_Path );
	void AddBone( const std::string& a_Name, int32_t a_Parent, const glm::mat4& a_Transform );
	void AddBone( const std::string& a_Name, const std::string& a_Parent, const glm::mat4& a_Transform );
	size_t GetBoneCount() const { return m_Bones.size(); }
	const Bone* GetBones() const { return m_Bones.data(); }
	const Bone* GetRootBone() const { return m_Bones.data(); }
	const Bone* operator[]( uint32_t a_Index ) const { return &m_Bones[ a_Index ]; }
	const Bone* operator[]( const std::string& a_Name ) const { auto Iter = m_Names.find( a_Name ); return Iter == m_Names.end() ? nullptr : &m_Bones[ Iter->second ]; }

private:

	std::map< std::string, uint32_t > m_Names;
	std::vector< Bone >               m_Bones;
};