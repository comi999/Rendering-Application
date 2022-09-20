#include <glm/ext.hpp>

#include "Light.hpp"
#include "Transform.hpp"
#include "Application.hpp"

Light::Light()
	: Radius( glm::radians( 30.0f ) )
	, Attentuation( 1.0f )
	, Distance( 1000000.0f )
	, Falloff( 0.0f )
	, Intensity( 1.0f )
	, Colour( 1.0f )
	, Direction( 0.5773f, -0.5773f, 0.5773f )
	, Position( 0.0f )
	, Type( LightType_Directional )
{ }

void Light::SetPointLight( float a_Distance, float a_Falloff = 1.0f )
{
	Radius = -1.0f;
	Attentuation = 0.0f;
	Distance = a_Distance;
	Falloff = a_Falloff;
	Position = { 0.0f, 0.0f, 0.0f };
	Type = LightType_Point;
}

void Light::SetDirectionalLight( const glm::vec3& a_Direction )
{
	Radius = 0.9f;
	Attentuation = 0.0f;
	Distance = 1000000.0f;
	Falloff = 0.0f;
	Direction = glm::normalize( a_Direction );
	Position = -Distance * glm::vec3{ Direction.x, Direction.y, Direction.z };
	Type = LightType_Directional;
}

void Light::SetSpotLight( float a_Distance, float a_Falloff, float a_Radius, float a_Attenuation )
{
	Radius = a_Radius;
	Attentuation = a_Attenuation;
	Distance = a_Distance;
	Falloff = a_Falloff;
	Position = { 0.0f, 0.0f, 0.0f };
	Type = LightType_Spot;
}

void Light::BuildMatrix( glm::mat4& o_Matrix ) const
{
	Transform* LightTransform = GetApplication()->GetComponent< Transform >( GetObject() );
	glm::mat4 Transformation = LightTransform->GetGlobal();
	glm::vec3 Pos = LightTransform->GetPosition();
	glm::vec3 Dir = glm::vec3( glm::mat4_cast( LightTransform->GetRotation() ) * glm::vec4( 0, 0, 1, 0 ) );

	// Characteristics
	o_Matrix[ 0 ][ 0 ] = Radius;
	o_Matrix[ 0 ][ 1 ] = Attentuation;
	o_Matrix[ 0 ][ 2 ] = Distance * Distance; // DistanceSqrd
	o_Matrix[ 0 ][ 3 ] = Falloff;

	// Colour
	o_Matrix[ 1 ][ 0 ] = Intensity * Colour.r;
	o_Matrix[ 1 ][ 1 ] = Intensity * Colour.g;
	o_Matrix[ 1 ][ 2 ] = Intensity * Colour.b;
	o_Matrix[ 1 ][ 3 ] = Type;

	// Position
	o_Matrix[ 2 ][ 0 ] = Pos.x;
	o_Matrix[ 2 ][ 1 ] = Pos.y;
	o_Matrix[ 2 ][ 2 ] = Pos.z;

	// Direction
	o_Matrix[ 3 ][ 0 ] = Dir.x;
	o_Matrix[ 3 ][ 1 ] = Dir.y;
	o_Matrix[ 3 ][ 2 ] = Dir.z;
}