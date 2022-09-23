#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Resource.hpp"

struct KeyPosition
{
	glm::vec3 Position;
	float     Time;
};

struct KeyRotation
{
	glm::quat Rotation;
	float     Time;
};

struct KeyScale
{
	glm::vec3 Scale;
	float     Time;
};

struct AnimationChannel
{
	std::string                Name;
	std::vector< KeyPosition > KeyPositions;
	std::vector< KeyRotation > KeyRotations;
	std::vector< KeyScale >    KeyScales;
};

class Animation : public Resource
{
public:

	Animation() = default;
	Animation( const std::string& a_Path );

private:

	float                           m_TicksPerSecond;
	float                           m_Duration;
	std::vector< AnimationChannel > m_Channels;
};