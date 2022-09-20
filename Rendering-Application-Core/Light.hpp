#pragma once
#include <glm/glm.hpp>

#include "Component.hpp"

enum LightType
{
	LightType_Directional,
	LightType_Point,
	LightType_Spot
};

class Light : public Component
{
public:

	float     Radius;
	float     Attentuation;
	float     Distance;
	float     Falloff;
	float     Intensity;
	glm::vec3 Colour;
	glm::vec3 Direction;
	glm::vec3 Position;
	LightType Type;

	Light();
	void SetPointLight( float a_Distance, float a_Attenuation );
	void SetDirectionalLight( const glm::vec3& a_Direction );
	void SetSpotLight( float a_Distance, float a_Falloff, float a_Radius, float a_Attenuation );
	void BuildMatrix( glm::mat4& o_Matrix ) const;
};