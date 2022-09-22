#pragma once
#include <glm/ext.hpp>

#include "Component.hpp"
#include "Application.hpp"
#include "Transform.hpp"
#include "CameraController.hpp"
#include "ObjectRotator.hpp"

class DemoController : public Component
{
public:

	void OnCreate()
	{
		m_Model0 = GetApplication()->Create();
		m_Model1 = GetApplication()->Create();
		GetApplication()->AddComponent< ObjectRotator >( m_Model0 )->Rotate = true;
		GetApplication()->AddComponent< ObjectRotator >( m_Model1 )->Rotate = true;
		GetApplication()->GetComponent< Transform >( m_Model1 )->SetPosition( { -2, 0, 0 } );
		GetApplication()->GetComponent< Transform >( m_Model1 )->SetScale( { 0.4, 0.4, 0.4 } );
		GetApplication()->GetComponent< Transform >( m_Model1 )->SetParent( GetApplication()->GetComponent< Transform >( m_Model0 ) );

		auto* t0 = GetApplication()->GetComponent< Transform >( m_Model0 );
		auto* t1 = GetApplication()->GetComponent< Transform >( m_Model1 );

		m_Camera = GetApplication()->Create();
		ComponentHandle< CameraController > Handle = GetApplication()->AddComponent< CameraController >( m_Camera );
		Handle = nullptr;
		Light* SomeLight = GetApplication()->AddComponent< Light >( m_Camera );
		SomeLight->SetPointLight( 0.0f, 0.0f );
		SomeLight->Intensity = 3.0f;
	}

	void OnTick( float a_DeltaTime )
	{
		if ( Input::KeyPressed( 'T' ) )
		{
			Transform* ThisTransform = GetApplication()->GetComponent< Transform >( m_Model1 );
			ThisTransform->SetParent( ThisTransform->GetParent() ? nullptr : GetApplication()->GetComponent< Transform >( m_Model0 ) );
		}
	}

	void OnDestroy()
	{
		GetApplication()->Destroy( m_Model0 );
		GetApplication()->Destroy( m_Model1 );
		GetApplication()->Destroy( m_Camera );
	}

private:

	Object m_Model0;
	Object m_Model1;
	Object m_Camera;
};