#pragma once
#include <vector>
#include <string>
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
	const Bone& operator[]( size_t a_Index ) const { return m_Bones[ a_Index ]; }

private:

	std::vector< Bone > m_Bones;
};