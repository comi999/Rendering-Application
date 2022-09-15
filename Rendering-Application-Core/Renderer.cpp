#include "Renderer.hpp"
#include "Rendering.hpp"
#include "Application.hpp"
#include "Transform.hpp"

void Renderer::OnCreate() { }

void Renderer::OnTick( float a_DeltaTime )
{
	
}

void Renderer::OnDestroy()
{

}

void Renderer::Submit()
{
	Application* ThisApplication = GetApplication();
	Transform* ThisTransform = ThisApplication->GetComponent< Transform >( GetObject() );
	DrawCall ToSubmit;
	ToSubmit.Mesh      = m_Mesh;
	ToSubmit.Material  = m_Material;
	ToSubmit.Transform = ThisTransform->GetTransformation();

	Rendering::Submit( ToSubmit );
}