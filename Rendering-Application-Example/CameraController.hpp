#pragma once
#include "Component.hpp"
#include "Input.hpp"
#include "Transform.hpp"
#include "Application.hpp"
#include "Light.hpp"

class CameraController : public Component
{
public:

	void OnCreate()
	{
		Camera* SomeCamera = GetApplication()->AddComponent< Camera >( GetObject() );
		GetApplication()->SetMainCamera( SomeCamera );
		Transform* CameraTransform = GetApplication()->GetComponent< Transform >( GetObject() );
		CameraTransform->SetPosition( { 0.0f, 2.0f, -5.0f } );

		Light* SomeLight = GetApplication()->AddComponent< Light >( GetObject() );
		SomeLight->SetDirectionalLight( { 0.0, 0.0, 1.0 } );
		SomeLight->Intensity = 1.0f;
	}

	void OnTick( float a_DeltaTime )
	{
		Transform* ThisTransform = GetApplication()->GetComponent< Transform >( GetObject() );
		glm::quat Rotation = ThisTransform->GetRotation();

		if ( Input::MouseButtonDown( 1 ) )
		{
			glm::vec2 MouseDelta = Input::GetMouseDelta() * a_DeltaTime;
			Rotation = glm::rotate( Rotation, MouseDelta.y, { 1.0f, 0.0f, 0.0f } );
			Rotation = glm::rotate( Rotation, MouseDelta.x, { 0.0f, 1.0f, 0.0f } );
			ThisTransform->SetRotation( Rotation );
		}

		glm::mat3 Forward{ glm::mat3_cast( Rotation ) };
		glm::vec3 Position{ 0.0f };
		if ( Input::KeyDown( 'A' ) ) Position.x -= a_DeltaTime;
		if ( Input::KeyDown( 'D' ) ) Position.x += a_DeltaTime;
		if ( Input::KeyDown( 'W' ) ) Position.z += a_DeltaTime;
		if ( Input::KeyDown( 'S' ) ) Position.z -= a_DeltaTime;
		if ( Input::KeyDown( 'Q' ) ) Position.y -= a_DeltaTime;
		if ( Input::KeyDown( 'E' ) ) Position.y += a_DeltaTime;

		ThisTransform->SetPosition( ThisTransform->GetPosition() + Forward * Position );
	}
};