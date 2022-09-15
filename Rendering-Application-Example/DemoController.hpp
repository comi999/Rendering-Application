#pragma once
#include <glm/ext.hpp>

#include "Component.hpp"
#include "Application.hpp"
#include "Transform.hpp"

class DemoController : public Component
{
public:

	void OnTick( float a_DeltaTime )
	{
		Transform* ThisTransform = GetApplication()->GetComponent< Transform >( GetObject() );
		glm::quat Rotation = ThisTransform->GetRotation();
		Rotation = glm::rotate( Rotation, a_DeltaTime, { 0, 1, 0 } );
		ThisTransform->SetRotation( Rotation );
	}
};