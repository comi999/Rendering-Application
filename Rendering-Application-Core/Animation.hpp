#pragma once
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

class Animation : public Resource
{
public:

	Animation() = default;
	Animation( const std::string& a_Path );

private:

	float m_TicksPerSecond;
	float m_Duration;
};