#pragma once
#include <vector>
#include <string>
#include <map>
#include <glm/glm.hpp>

#include "Resource.hpp"

struct Bone
{
	std::string Name;
	int32_t     Parent;
	glm::mat4   Local;
	glm::mat4   Offset;
};

class Skeleton : public Resource
{
public:

	Skeleton() = default;
	Skeleton( const std::string& a_Path );
	size_t GetBoneCount() const { return m_Bones.size(); }
	const Bone* GetBones() const { return m_Bones.data(); }
	const Bone& operator[]( uint32_t a_Index ) const { return m_Bones[ a_Index ]; }

private:

	std::vector< Bone > m_Bones;
};