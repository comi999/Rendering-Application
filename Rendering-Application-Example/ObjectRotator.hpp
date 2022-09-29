#pragma once
#include <glm/glm.hpp>

#include "Component.hpp"
#include "Application.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"
#include "Transform.hpp"
#include "Object.hpp"
#include "Skeleton.hpp"
#include "Animation.hpp"
#include "Animator.hpp"

class ObjectRotator : public Component
{
public:

	bool Rotate = false;

	void OnTick( float a_DeltaTime )
	{
		Transform* ThisTransform = GetApplication()->GetComponent< Transform >( GetObject() );

		if ( Rotate )
		{
			glm::quat Rotation = ThisTransform->GetRotation();
			Rotation = glm::rotate( Rotation, a_DeltaTime, { 0, 1, 0 } );
			ThisTransform->SetRotation( Rotation );
		}
	}
};