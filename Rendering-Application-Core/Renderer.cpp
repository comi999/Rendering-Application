#include "Renderer.hpp"
#include "Rendering.hpp"
#include "Application.hpp"
#include "Transform.hpp"

void Renderer::OnRender()
{
	Application* ThisApplication = GetApplication();
	Transform* ThisTransform = ThisApplication->GetComponent< Transform >( GetObject() );
	DrawCall ToSubmit;
	ToSubmit.Mesh      = m_Mesh;
	ToSubmit.Material  = m_Material;
	ToSubmit.RenderingMode = RenderingMode_Triangle;
	ToSubmit.Transform = ThisTransform->GetGlobal();

	Rendering::Submit( ToSubmit );
}