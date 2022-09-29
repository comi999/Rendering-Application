#include "LineRenderer.hpp"
#include "Rendering.hpp"
#include "Application.hpp"
#include "Transform.hpp"

void LineRenderer::Submit()
{
	static Mesh s_LineMesh;
	static Material s_LineMaterial;
	static Shader s_LineShader;
	static Utility::OnStart s_OnStart = []()
	{
		s_LineMesh.AddPosition( { 1.0f, 0.0f, 0.0f, 1.0f } );
		s_LineMesh.AddPosition( { 0.0f, 0.0f, 0.0f, 1.0f } );
		s_LineMesh.AddPosition( { 0.0f, 0.0f, 0.0f, 1.0f } );
		s_LineMesh.AddColour( { 1.0f, 1.0f, 1.0f, 1.0f } );
		s_LineMesh.AddColour( { 1.0f, 1.0f, 1.0f, 1.0f } );
		s_LineMesh.AddColour( { 1.0f, 1.0f, 1.0f, 1.0f } );
		s_LineMesh.AddIndex( 0 );
		s_LineMesh.AddIndex( 1 );
		s_LineMesh.AddIndex( 2 );

		s_LineShader = Shader( "Resources/LineRenderer.shader" );
		s_LineShader.Compile();

		s_LineMaterial.SetShader( &s_LineShader );
	};


	Application* ThisApplication = GetApplication();
	Transform* ThisTransform = ThisApplication->GetComponent< Transform >( GetObject() );
	DrawCall ToSubmit;
	ToSubmit.Mesh = &s_LineMesh;
	ToSubmit.Material = &s_LineMaterial;
	ToSubmit.Transform = ThisTransform->GetGlobal();

	Rendering::Submit( ToSubmit );
}