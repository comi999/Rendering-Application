#include "Renderer.hpp"
#include "Rendering.hpp"
#include "Application.hpp"
#include "Transform.hpp"

void Renderer::Submit()
{
	Application* ThisApplication = GetApplication();
	Transform* ThisTransform = ThisApplication->GetComponent< Transform >( GetObject() );
	DrawCall ToSubmit;
	ToSubmit.Mesh      = m_Mesh;
	ToSubmit.Material  = m_Material;
	ToSubmit.Transform = ThisTransform->GetGlobal();

	Rendering::Submit( ToSubmit );
}