#pragma once
#include <glm/ext.hpp>

#include "Component.hpp"
#include "Application.hpp"
#include "Transform.hpp"
#include "CameraController.hpp"
#include "ObjectRotator.hpp"
#include "SoulSpearDemo.hpp"
#include "AnimationDemo.hpp"
#include "LineRenderer.hpp"

class DemoController : public Component
{
public:

	void OnCreate()
	{
		// Create two objects.
		m_Model0 = GetApplication()->Create();
		m_Model1 = GetApplication()->Create();

		// Add the demo components to them.
		//GetApplication()->AddComponent< SoulSpearDemo >( m_Model0 );
		GetApplication()->AddComponent< AnimationDemo >( m_Model1 );
		/*LineRenderer* SomeLineRenderer0 = GetApplication()->AddComponent< LineRenderer >( GetApplication()->Create() );
		LineRenderer* SomeLineRenderer1 = GetApplication()->AddComponent< LineRenderer >( GetApplication()->Create() );
		LineRenderer* SomeLineRenderer2 = GetApplication()->AddComponent< LineRenderer >( GetApplication()->Create() );
		LineRenderer* SomeLineRenderer3 = GetApplication()->AddComponent< LineRenderer >( GetApplication()->Create() );

		GetApplication()->GetComponent< Transform >( SomeLineRenderer1->GetObject() )->SetPosition( { 2, 0, 0 } );
		GetApplication()->GetComponent< Transform >( SomeLineRenderer2->GetObject() )->SetPosition( { 2, 0, 0 } );
		GetApplication()->GetComponent< Transform >( SomeLineRenderer3->GetObject() )->SetPosition( { 2, 0, 0 } );

		GetApplication()->GetComponent< Transform >( SomeLineRenderer1->GetObject() )->SetParent( GetApplication()->GetComponent< Transform >( SomeLineRenderer0->GetObject() ), false );
		GetApplication()->GetComponent< Transform >( SomeLineRenderer2->GetObject() )->SetParent( GetApplication()->GetComponent< Transform >( SomeLineRenderer1->GetObject() ), false );
		GetApplication()->GetComponent< Transform >( SomeLineRenderer3->GetObject() )->SetParent( GetApplication()->GetComponent< Transform >( SomeLineRenderer2->GetObject() ), false );*/
		
		//SomeLineRenderer->Start = glm::vec3( 0, 0, 0 );
		//SomeLineRenderer->End = glm::vec3( 1, 1, 1 );


		// Add camera to DemoController object
		m_Camera = GetApplication()->Create();
		GetApplication()->AddComponent< CameraController >( m_Camera );

		// Add light to DemoController object
		Light* SomeLight = GetApplication()->AddComponent< Light >( m_Camera );
		SomeLight->SetPointLight( 0.0f, 0.0f );
		SomeLight->Intensity = 3.0f;
	}

	void OnTick( float a_DeltaTime )
	{

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